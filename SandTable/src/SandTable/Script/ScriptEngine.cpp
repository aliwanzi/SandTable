#include "pch.h"
#include "ScriptEngine.h"
#include "ScriptGlue.h"
#include "ScriptInstance.h"
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

		Ref<MonoAssembly> CoreAssembly = nullptr;
		Ref<MonoImage> CoreMonoImage = nullptr;

		Ref<MonoAssembly> AppAssembly = nullptr;
		Ref<MonoImage> AppMonoImage = nullptr;

		Ref<MonoDomain> ScriptDomain = nullptr;
		Ref<ScriptClass> ScriptEntityClass;
		std::unordered_map<std::string, Ref<ScriptClass>> ScriptClass;
		std::unordered_map<UUID, Ref<ScriptInstance>> ScriptInstance;

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

	std::string ScriptFieldTypeToString(const ScriptFieldType& eFieldType)
	{
		switch (eFieldType)
		{
			case ScriptFieldType::None:    return "None";
			case ScriptFieldType::Float:   return "Float";
			case ScriptFieldType::Double:  return "Double";
			case ScriptFieldType::Bool:    return "Bool";
			case ScriptFieldType::Char:    return "Char";
			case ScriptFieldType::Byte:    return "Byte";
			case ScriptFieldType::Short:   return "Short";
			case ScriptFieldType::Int:     return "Int";
			case ScriptFieldType::Long:    return "Long";
			case ScriptFieldType::UByte:   return "UByte";
			case ScriptFieldType::UShort:  return "UShort";
			case ScriptFieldType::UInt:    return "UInt";
			case ScriptFieldType::ULong:   return "ULong";
			case ScriptFieldType::Vector2: return "Vector2";
			case ScriptFieldType::Vector3: return "Vector3";
			case ScriptFieldType::Vector4: return "Vector4";
			case ScriptFieldType::Entity:  return "Entity";
		}
		SAND_TABLE_ASSERT(false, "Unknown ScriptFieldType");
		return "None";
	}
}

void ScriptEngine::Init()
{
	InitMono();
	LoadCoreAssembly("assets/script/SandTableScript.dll");
	LoadAppAssembly("assets/script/SandBoxScript.dll");

	ScriptGlue::RegisterFunctions();
	ScriptGlue::RegisterComponents();
	LoadAssemblyClass();
}

void ScriptEngine::InitMono()
{
	mono_set_assemblies_path("mono/lib");

	spScriptEngineData = CreateRef<ScriptEngineData>();
	spScriptEngineData->RootDomain = Ref<MonoDomain>(mono_jit_init("SandTableScriptRuntime"));
	SAND_TABLE_ASSERT(spScriptEngineData->RootDomain, "RootDomain is null in InitMono");
}

void ScriptEngine::LoadCoreAssembly(const std::filesystem::path& sAssemblyPath)
{
	spScriptEngineData->ScriptDomain = Ref<MonoDomain>(mono_domain_create_appdomain("SandTableScriptRuntime", nullptr));
	mono_domain_set(spScriptEngineData->ScriptDomain.get(), true);

	LoadAssembly(sAssemblyPath, spScriptEngineData->CoreAssembly, spScriptEngineData->CoreMonoImage);
}

void ScriptEngine::LoadAppAssembly(const std::filesystem::path& sAssemblyPath)
{
	LoadAssembly(sAssemblyPath, spScriptEngineData->AppAssembly, spScriptEngineData->AppMonoImage);
}

bool ScriptEngine::EntityClassExit(const std::string& sFullClassName)
{
	return spScriptEngineData->ScriptClass.find(sFullClassName) != spScriptEngineData->ScriptClass.end();
}

const std::unordered_map<std::string, Ref<ScriptClass>>& ScriptEngine::GetEntityClass()
{
	return spScriptEngineData->ScriptClass;
}

Ref<ScriptInstance> ScriptEngine::GetEntityScriptInstance(UUID uiEntityID)
{
	auto iter = spScriptEngineData->ScriptInstance.find(uiEntityID);
	if (iter != spScriptEngineData->ScriptInstance.end())
	{
		return iter->second;
	}
	return nullptr;
}

void ScriptEngine::OnCreateEntity(Ref<Entity> spEntity)
{
	const auto& scriptComponent = spEntity->GetComponent<ScriptComponent>();
	if (EntityClassExit(scriptComponent.ClassName))
	{
		auto spScriptInstance = CreateRef<ScriptInstance>(spScriptEngineData->ScriptEntityClass,
			spScriptEngineData->ScriptClass[scriptComponent.ClassName], spEntity->GetUUID());
		spScriptEngineData->ScriptInstance[spEntity->GetUUID()] = spScriptInstance;
		spScriptInstance->InvokeOnCreate();
	}
}

void ScriptEngine::OnUpdateEntity(Ref<Entity> spEntity, TimeStep fTimeStep)
{
	auto spScriptInstance = spScriptEngineData->ScriptInstance.find(spEntity->GetUUID());
	SAND_TABLE_ASSERT(spScriptInstance != spScriptEngineData->ScriptInstance.end(), "not find Script Instance");

	spScriptInstance->second->InVokeOnUpdate(fTimeStep);
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
	spScriptEngineData->ScriptInstance.clear();
}

void ScriptEngine::LoadAssemblyClass()
{
	spScriptEngineData->ScriptEntityClass = CreateRef<ScriptClass>("SandTable", "Entity",
		spScriptEngineData->ScriptDomain, spScriptEngineData->CoreMonoImage);

	const auto& pMonoImage = spScriptEngineData->AppMonoImage.get();
	const MonoTableInfo* pTypeDefinitionsTable = mono_image_get_table_info(pMonoImage, MONO_TABLE_TYPEDEF);
	int iNumTypes = mono_table_info_get_rows(pTypeDefinitionsTable);

	MonoClass* pEntityClass = mono_class_from_name(spScriptEngineData->CoreMonoImage.get(), "SandTable", "Entity");

	for (int i = 0; i < iNumTypes; i++)
	{
		auto spCols = Ref<uint32_t>(new uint32_t[MONO_TYPEDEF_SIZE], std::default_delete<uint32_t>());
		mono_metadata_decode_row(pTypeDefinitionsTable, i, spCols.get(), MONO_TYPEDEF_SIZE);

		const char* pClassNameSpace = mono_metadata_string_heap(pMonoImage, spCols.get()[MONO_TYPEDEF_NAMESPACE]);
		const char* pClassName = mono_metadata_string_heap(pMonoImage, spCols.get()[MONO_TYPEDEF_NAME]);
		std::string sFullName("");
		if (strlen(pClassNameSpace) != 0)
		{
			sFullName = fmt::format("{}.{}", pClassNameSpace, pClassName);
		}
		else
		{
			sFullName = pClassName;
		}

		MonoClass* pMonoClass = mono_class_from_name(pMonoImage, pClassNameSpace, pClassName);
		if (pMonoClass == pEntityClass)
		{
			continue;
		}

		if (!mono_class_is_subclass_of(pMonoClass, pEntityClass, false))
		{
			continue;
		}

		auto spScriptClass = CreateRef<ScriptClass>(pClassNameSpace, pClassName,
			spScriptEngineData->ScriptDomain, spScriptEngineData->AppMonoImage);

		spScriptEngineData->ScriptClass[sFullName] = spScriptClass;

		int iFiledCount = mono_class_num_fields(pMonoClass);

		LOG_DEV_INFO("{}.{} has {} fileds: ", pClassNameSpace, pClassName, iFiledCount);
		void* iterator = nullptr;
		while (MonoClassField* pField = mono_class_get_fields(pMonoClass, &iterator))
		{
			const char* pFieldName = mono_field_get_name(pField);
			uint32_t uiFlags = mono_field_get_flags(pField);
			LOG_DEV_INFO(" {} flags = {}", pFieldName, uiFlags);
			if (uiFlags & FIELD_ATTRIBUTE_PUBLIC)
			{
				ScriptFieldType eFileType = MonoTypeToScriptFieldType(mono_field_get_type(pField));
				LOG_DEV_INFO(" {}({}) is public", pFieldName, ScriptFieldTypeToString(eFileType));
				spScriptClass->AddScriptField(pFieldName, { eFileType,pField });
			}
		}
	}


}

const Ref<MonoImage>& ScriptEngine::GetCoreAssemblyImage()
{
	return spScriptEngineData->CoreMonoImage;
}

SAND_TABLE_NAMESPACE_END