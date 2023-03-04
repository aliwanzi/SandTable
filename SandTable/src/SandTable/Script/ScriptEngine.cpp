#include "pch.h"
#include "ScriptEngine.h"
#include "ScriptGlue.h"
#include "ScriptEntityInstance.h"
#include "ScriptEntityClass.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/tabledefs.h"

#include "SandTable/Scene/Scene.h"
#include "SandTable/Scene/Entity.h"

SAND_TABLE_NAMESPACE_BEGIN

namespace
{
	struct ScriptEngineData
	{  
		Ref<MonoDomain> RootDomain = nullptr;
		Ref<MonoDomain> AppDomain = nullptr;

		Ref<MonoAssembly> CoreAssembly = nullptr;
		Ref<MonoImage> CoreMonoImage = nullptr;

		Ref<MonoAssembly> AppAssembly = nullptr;
		Ref<MonoImage> AppMonoImage = nullptr;

		Ref<ScriptEntityClass> CoreScriptEntityClass;
		MapScriptEntityClass AppScriptEntityClassMap;

		MapScriptEntityInstance ScriptEntityInstanceMap;
		std::unordered_map<UUID, MapScriptField> ScriptFieldMap;

		//
		Ref<Scene> SceneContext;
	};
	static Ref<ScriptEngineData> spScriptEngineData;

	void LoadAssembly(const std::filesystem::path& sAssemblyPath, Ref<MonoAssembly>& spMonoAssembly, Ref<MonoImage>& spMonoImage)
	{
		std::ifstream stream(sAssemblyPath, std::ios::binary | std::ios::ate);
		if (!stream)
		{
			SAND_TABLE_ASSERT(false, "Read Assembly {0} Failed", sAssemblyPath);;
		}

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t iFileSize = static_cast<uint32_t>(end - stream.tellg());
		if (iFileSize == 0)
		{
			SAND_TABLE_ASSERT(false, "Read Assembly {0} is Null", sAssemblyPath);
		}

		auto spFileData = Ref<char>(new char[iFileSize], std::default_delete<char>());
		stream.read(spFileData.get(), iFileSize);
		stream.close();

		MonoImageOpenStatus status;
		MonoImage* pMonoImage = mono_image_open_from_data_full(spFileData.get(), iFileSize, true, &status, false);
		if (status != MONO_IMAGE_OK)
		{
			SAND_TABLE_ASSERT(false, mono_image_strerror(status));
		}

		spMonoAssembly = Ref<MonoAssembly>(mono_assembly_load_from_full(pMonoImage, sAssemblyPath.string().c_str(), &status, false));
		if (status != MONO_IMAGE_OK)
		{
			SAND_TABLE_ASSERT(false, mono_image_strerror(status));
		}

		spMonoImage = Ref<MonoImage>(mono_assembly_get_image(spMonoAssembly.get()));
		mono_image_close(pMonoImage);
	}

	ScriptFieldType MonoTypeToScriptFieldType(MonoType* pMonoType)
	{
		std::string sTypeName = mono_type_get_name(pMonoType);
		SAND_TABLE_ASSERT(ScriptFieldTypeMap.find(sTypeName) != ScriptFieldTypeMap.end(),"do not find MonoType in ScriptEngine");
		return ScriptFieldTypeMap.at(sTypeName);
	}
}

void ScriptEngine::Init()
{
	InitMono();
	LoadAssemblyAndMonoImage();

	ScriptGlue::RegisterFunctions();
	ScriptGlue::RegisterComponents();

	LoadAssemblyClass();
}

void ScriptEngine::InitMono()
{
	mono_set_assemblies_path("script/mono/lib");

	spScriptEngineData = CreateRef<ScriptEngineData>();
	spScriptEngineData->RootDomain = Ref<MonoDomain>(mono_jit_init("SandTableScriptRuntime"));
	SAND_TABLE_ASSERT(spScriptEngineData->RootDomain, "RootDomain is null in InitMono");

	spScriptEngineData->AppDomain = Ref<MonoDomain>(mono_domain_create_appdomain("SandTableScriptRuntime", nullptr));
	SAND_TABLE_ASSERT(spScriptEngineData->AppDomain, "AppDomain is null in InitMono");
	mono_domain_set(spScriptEngineData->AppDomain.get(), true);
}

void ScriptEngine::LoadAssemblyAndMonoImage()
{
	LoadAssembly("script/SandTableScript.dll", spScriptEngineData->CoreAssembly, spScriptEngineData->CoreMonoImage);
	LoadAssembly("script/SandBoxScript.dll", spScriptEngineData->AppAssembly, spScriptEngineData->AppMonoImage);
}

void ScriptEngine::OnCreateEntity(Ref<Entity> spEntity)
{
	const auto& scriptComponent = spEntity->GetComponent<ScriptComponent>();
	if (ScriptEntityClassExit(scriptComponent.ClassName))
	{
		auto uuid = spEntity->GetUUID();
		auto spScriptInstance = CreateRef<ScriptEntityInstance>(spScriptEngineData->CoreScriptEntityClass,
			spScriptEngineData->AppScriptEntityClassMap[scriptComponent.ClassName], uuid);

		spScriptEngineData->ScriptEntityInstanceMap[uuid] = spScriptInstance;
		spScriptInstance->InvokeOnCreate();

		auto iter = spScriptEngineData->ScriptFieldMap.find(uuid);
		if (!iter->second.empty())
		{
			for (auto field : iter->second)
			{
				spScriptInstance->GetAppScriptEntityClass()->SetFieldValueBuffer
				(field.first, field.second.FieldValue->GetBuffer());
			}
		}
	}
}

void ScriptEngine::OnUpdateEntity(Ref<Entity> spEntity, TimeStep fTimeStep)
{
	auto iter = spScriptEngineData->ScriptEntityInstanceMap.find(spEntity->GetUUID());
	SAND_TABLE_ASSERT(iter != spScriptEngineData->ScriptEntityInstanceMap.end(), "not find Script Instance");

	iter->second->InVokeOnUpdate(fTimeStep);
}

const MapScriptEntityClass& ScriptEngine::GetScriptEntityClassMap()
{
	return spScriptEngineData->AppScriptEntityClassMap;
}


bool ScriptEngine::ScriptEntityClassExit(const std::string& sEntityName)
{
	return spScriptEngineData->AppScriptEntityClassMap.find(sEntityName) 
		!= spScriptEngineData->AppScriptEntityClassMap.end();
}

Ref<ScriptEntityClass> ScriptEngine::GetScriptEntityClass(const std::string& sEntityName)
{
	auto iter = spScriptEngineData->AppScriptEntityClassMap.find(sEntityName);
	if (iter != spScriptEngineData->AppScriptEntityClassMap.end())
	{
		return iter->second;
	}
	else
	{
		return nullptr;
	}
}

Ref<ScriptEntityInstance> ScriptEngine::GetScriptEntityInstance(const UUID& uiEntityID)
{
	auto iter = spScriptEngineData->ScriptEntityInstanceMap.find(uiEntityID);
	if (iter != spScriptEngineData->ScriptEntityInstanceMap.end())
	{
		return iter->second;
	}
	return nullptr;
}

MapScriptField& ScriptEngine::GetScriptFieldMap(const UUID& uiEntityID)
{
	return spScriptEngineData->ScriptFieldMap[uiEntityID];
}

void ScriptEngine::OnRuntimeStart(Ref<Scene> spScene)
{
	spScriptEngineData->SceneContext = spScene;
}

const Ref<Scene>& ScriptEngine::GetRuntimeScene()
{
	return spScriptEngineData->SceneContext;
}

void ScriptEngine::OnRuntimeStop()
{
	spScriptEngineData->SceneContext = nullptr;
	spScriptEngineData->ScriptEntityInstanceMap.clear();
}

void ScriptEngine::LoadAssemblyClass()
{
	spScriptEngineData->CoreScriptEntityClass = CreateRef<ScriptEntityClass>("SandTable", "Entity",
		spScriptEngineData->CoreMonoImage, spScriptEngineData->AppDomain);
	MonoClass* pCoreClass = mono_class_from_name(spScriptEngineData->CoreMonoImage.get(), 
		"SandTable", "Entity");

	const auto& pMonoImage = spScriptEngineData->AppMonoImage.get();
	const MonoTableInfo* pClassNameType = mono_image_get_table_info(pMonoImage, MONO_TABLE_TYPEDEF);
	int iClassNum = mono_table_info_get_rows(pClassNameType);

	for (int i = 0; i < iClassNum; i++)
	{
		auto spTableInfos = Ref<uint32_t>(new uint32_t[MONO_TYPEDEF_SIZE], std::default_delete<uint32_t>());
		mono_metadata_decode_row(pClassNameType, i, spTableInfos.get(), MONO_TYPEDEF_SIZE);

		const char* pClassNameSpace = mono_metadata_string_heap(pMonoImage, spTableInfos.get()[MONO_TYPEDEF_NAMESPACE]);
		const char* pClassName = mono_metadata_string_heap(pMonoImage, spTableInfos.get()[MONO_TYPEDEF_NAME]);
		std::string sEntityClassName("");
		if (strlen(pClassNameSpace) != 0)
		{
			sEntityClassName = fmt::format("{}.{}", pClassNameSpace, pClassName);
		}
		else
		{
			sEntityClassName = pClassName;
		}

		MonoClass* pAppClass = mono_class_from_name(pMonoImage, pClassNameSpace, pClassName);
		if (pAppClass == pCoreClass)
		{
			continue;
		}

		if (!mono_class_is_subclass_of(pAppClass, pCoreClass, false))
		{
			continue;
		}

		auto spScriptClass = CreateRef<ScriptEntityClass>(pClassNameSpace, pClassName,
			spScriptEngineData->AppMonoImage, spScriptEngineData->AppDomain);

		spScriptEngineData->AppScriptEntityClassMap[sEntityClassName] = spScriptClass;

		int iFiledCount = mono_class_num_fields(pAppClass);

		LOG_DEV_INFO("{}.{} has {} fileds: ", pClassNameSpace, pClassName, iFiledCount);
		void* iterator = nullptr;
		while (MonoClassField* pFieldMonoClass = mono_class_get_fields(pAppClass, &iterator))
		{
			const char* pFieldName = mono_field_get_name(pFieldMonoClass);
			uint32_t uiFlags = mono_field_get_flags(pFieldMonoClass);
			LOG_DEV_INFO(" {} flags = {}", pFieldName, uiFlags);
			if (uiFlags & FIELD_ATTRIBUTE_PUBLIC)
			{
				ScriptFieldType eFileType = MonoTypeToScriptFieldType(mono_field_get_type(pFieldMonoClass));
				LOG_DEV_INFO(" {}({}) is public", pFieldName, ScriptFieldTypeToString(eFileType));
				auto spFiledValue = CreateRef<ScriptFieldValue>();
				spScriptClass->AddScriptEntityField(pFieldName, { eFileType,spFiledValue,pFieldMonoClass });
			}
		}
	}
}

const Ref<MonoImage>& ScriptEngine::GetCoreAssemblyImage()
{
	return spScriptEngineData->CoreMonoImage;
}

SAND_TABLE_NAMESPACE_END