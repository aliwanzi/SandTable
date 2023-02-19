#include "pch.h"
#include "ScriptEngine.h"
#include "ScriptGlue.h"
#include "ScriptClass.h"
#include "ScriptInstance.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"

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

		Ref<ScriptClass> EntityScriptClass;
		std::unordered_map<std::string, Ref<ScriptClass>> EntityClass;
		std::unordered_map<UUID, Ref<ScriptInstance>> EntityInstance;

		//
		Ref<Scene> SceneContext;
	};
	static Ref<ScriptEngineData> spScriptEngineData;
}

void ScriptEngine::Init()
{
	InitMono();
	LoadAssembly("Resources/Scripts/SandTable-ScriptCore.dll");

	ScriptGlue::RegisterFunctions();
	LoadAssemblyClass();
	InvokeClass("SandTable", "Entity");
}

void ScriptEngine::InitMono()
{
	mono_set_assemblies_path("mono/lib");

	spScriptEngineData = CreateRef<ScriptEngineData>();
	spScriptEngineData->RootDomain = Ref<MonoDomain>(mono_jit_init("SandTableScriptRuntime"));
	SAND_TABLE_ASSERT(spScriptEngineData->RootDomain, "RootDomain is null in InitMono");
}

void ScriptEngine::LoadAssembly(const std::filesystem::path& sAssemblyPath)
{
	spScriptEngineData->AppDomain = Ref<MonoDomain>(mono_domain_create_appdomain("SandTableScriptRuntime", nullptr));
	mono_domain_set(spScriptEngineData->AppDomain.get(), true);

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

	spScriptEngineData->CoreAssembly = Ref<MonoAssembly>(mono_assembly_load_from_full(pMonoImage, sAssemblyPath.string().c_str(), &status, false));
	mono_image_close(pMonoImage);
	if (status != MONO_IMAGE_OK)
	{
		SAND_TABLE_ASSERT(false, mono_image_strerror(status));
	}

	spScriptEngineData->CoreMonoImage = Ref<MonoImage>(mono_assembly_get_image(spScriptEngineData->CoreAssembly.get()));
}

bool ScriptEngine::EntityClassExit(const std::string& sFullClassName)
{
	return spScriptEngineData->EntityClass.find(sFullClassName) != spScriptEngineData->EntityClass.end();
}

const std::unordered_map<std::string, Ref<ScriptClass>>& ScriptEngine::GetEntityClass()
{
	return spScriptEngineData->EntityClass;
}

void ScriptEngine::OnCreateEntity(Ref<Entity> spEntity)
{
	const auto& scriptComponent = spEntity->GetComponent<ScriptComponent>();
	if (EntityClassExit(scriptComponent.ClassName))
	{
		auto spScriptInstance = CreateRef<ScriptInstance>(spScriptEngineData->EntityClass[scriptComponent.ClassName]);
		spScriptEngineData->EntityInstance[spEntity->GetUUID()] = spScriptInstance;
		spScriptInstance->InvokeOnCreate();
	}
}

void ScriptEngine::OnUpdateEntity(Ref<Entity> spEntity, TimeStep fTimeStep)
{
	auto spScriptInstance = spScriptEngineData->EntityInstance.find(spEntity->GetUUID());
	SAND_TABLE_ASSERT(spScriptInstance != spScriptEngineData->EntityInstance.end(), "not find Script Instance");

	spScriptInstance->second->InVokeOnUpdate(fTimeStep);
}

void ScriptEngine::OnRuntimeStart(Ref<Scene> spScene)
{
	spScriptEngineData->SceneContext = spScene;
}

void ScriptEngine::OnRuntimeStop()
{
	spScriptEngineData->SceneContext = nullptr;
	spScriptEngineData->EntityInstance.clear();
}

void ScriptEngine::InvokeClass(const std::string& sClassNameSpace, const std::string& sClassName)
{
	spScriptEngineData->EntityScriptClass = CreateRef<ScriptClass>(sClassNameSpace, sClassName,
		spScriptEngineData->AppDomain, spScriptEngineData->CoreMonoImage);

	auto pPrintMessageFunc = spScriptEngineData->EntityScriptClass->GetMonoMethod("PrintMessage", 0);
	spScriptEngineData->EntityScriptClass->InvokeMethod(pPrintMessageFunc);

	auto spPrintIntFunc = spScriptEngineData->EntityScriptClass->GetMonoMethod("PrintInt", 1);
	int iValue1(5);
	void* pParam = &iValue1;
	spScriptEngineData->EntityScriptClass->InvokeMethod(spPrintIntFunc, &pParam);

	auto spPrintIntsFunc = spScriptEngineData->EntityScriptClass->GetMonoMethod("PrintInts", 2);
	int iValue2(10);
	void* pParams[2] = {
		&iValue1,
		&iValue2
	};
	spScriptEngineData->EntityScriptClass->InvokeMethod(spPrintIntsFunc, pParams);

	MonoString* pMonoString = mono_string_new(spScriptEngineData->AppDomain.get(), "Hello World from c++");
	auto spPrintCustomMessageFunc = spScriptEngineData->EntityScriptClass->GetMonoMethod("PrintCustomMessage", 1);
	void* sParam = pMonoString;
	spScriptEngineData->EntityScriptClass->InvokeMethod(spPrintCustomMessageFunc,&sParam);
}


void ScriptEngine::LoadAssemblyClass()
{
	const auto& pMonoImage = spScriptEngineData->CoreMonoImage.get();
	const MonoTableInfo* pTypeDefinitionsTable = mono_image_get_table_info(pMonoImage, MONO_TABLE_TYPEDEF);
	int iNumTypes = mono_table_info_get_rows(pTypeDefinitionsTable);

	MonoClass* pEntityClass = mono_class_from_name(pMonoImage, "SandTable", "Entity");

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

		spScriptEngineData->EntityClass[sFullName] = CreateRef<ScriptClass>(pClassNameSpace, pClassName,
			spScriptEngineData->AppDomain, spScriptEngineData->CoreMonoImage);

		LOG_DEV_INFO("{}.{}", pClassNameSpace, pClassName);
	}
}

SAND_TABLE_NAMESPACE_END