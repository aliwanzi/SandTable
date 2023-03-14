#include "pch.h"
#include "ScriptEngine.h"
#include "ScriptGlue.h"

#include "ScriptEntityInstance.h"
#include "ScriptEntityClass.h"

#include "SandTable/Core/Application.h"
#include "SandTable/Core/FileSystem.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/tabledefs.h"
#include "mono/metadata/mono-debug.h"
#include "mono/metadata/threads.h"

#include "SandTable/Scene/Scene.h"
#include "SandTable/Scene/Entity.h"

#include "SandTable/Project/Project.h"

#include "FileWatch.h"

SAND_TABLE_NAMESPACE_BEGIN

namespace
{
	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreMonoImage = nullptr;

		MonoAssembly* AppAssembly = nullptr;
		MonoImage* AppMonoImage = nullptr;

		Ref<ScriptEntityClass> CoreScriptEntityClass;
		MapScriptEntityClass AppScriptEntityClassMap;

		MapScriptEntityInstance ScriptEntityInstanceMap;
		std::unordered_map<UUID, MapScriptField> ScriptFieldMap;

		Ref<Scene> SceneContext;

		Scope<filewatch::FileWatch<std::string>> AppAssemblyFileWatcher;
		bool AssemblyReloadPending = false;

		bool EnableDebugging = true;

		~ScriptEngineData()
		{
			LOG_DEV_INFO("ScriptEngineData Dtor");
			mono_domain_set(mono_get_root_domain(), false);

			mono_domain_unload(AppDomain);
			AppDomain = nullptr;

			mono_jit_cleanup(RootDomain);
			RootDomain = nullptr;
		}
	};
	static Ref<ScriptEngineData> spScriptEngineData;

	void LoadAssembly(const std::filesystem::path& sAssemblyPath, MonoAssembly*& pMonoAssembly, MonoImage*& pMonoImage)
	{
		auto spAssemblyData = FileSystem::ReadFileDataBuffer(sAssemblyPath);
		if (spAssemblyData == nullptr)
		{
			return;
		}
		MonoImageOpenStatus status;
		MonoImage* MonoImage = mono_image_open_from_data_full(spAssemblyData->As<char>(), spAssemblyData->GetDataBufferSize(), true, &status, false);
		if (status != MONO_IMAGE_OK)
		{
			SAND_TABLE_ASSERT(false, mono_image_strerror(status));
		}

		if (spScriptEngineData->EnableDebugging)
		{
			std::filesystem::path sPdbPath = sAssemblyPath;
			sPdbPath.replace_extension(".pdb");
			if (std::filesystem::exists(sPdbPath))
			{
				auto spPdbData = FileSystem::ReadFileDataBuffer(sPdbPath);
				mono_debug_open_image_from_memory(MonoImage, spPdbData->As<const mono_byte>(), spPdbData->GetDataBufferSize());
				LOG_DEV_INFO("Trying to load pdb: {}", sPdbPath);
			}
		}

		pMonoAssembly = mono_assembly_load_from_full(MonoImage, sAssemblyPath.string().c_str(), &status, false);
		if (status != MONO_IMAGE_OK)
		{
			SAND_TABLE_ASSERT(false, mono_image_strerror(status));
		}

		pMonoImage = mono_assembly_get_image(pMonoAssembly);
		mono_image_close(pMonoImage);
	}

	ScriptFieldType MonoTypeToScriptFieldType(MonoType* pMonoType)
	{
		std::string sTypeName = mono_type_get_name(pMonoType);
		SAND_TABLE_ASSERT(ScriptFieldTypeMap.find(sTypeName) != ScriptFieldTypeMap.end(), "do not find MonoType in ScriptEngine");
		return ScriptFieldTypeMap.at(sTypeName);
	}

	void OnAppAssemblyFileSystemEvent(const std::string& sFilePath, const filewatch::Event eventType)
	{
		LOG_DEV_INFO("Path is {0}, Event Type is {1}", sFilePath, int(eventType));
		if (!spScriptEngineData->AssemblyReloadPending && eventType == filewatch::Event::modified)
		{
			spScriptEngineData->AssemblyReloadPending = true;
			Application::GetApplication()->SubmitToMainThreadQueue([]()
				{
					spScriptEngineData->AppAssemblyFileWatcher.reset();
					ScriptEngine::ReloadAssembly();
				});
		}
	}
}

void ScriptEngine::Init()
{
	InitMono();
	if (!LoadAssemblyAndMonoImage())
	{
		return;
	}

	ScriptGlue::RegisterFunctions();
	ScriptGlue::RegisterComponents();

	LoadAssemblyClass();
}

void ScriptEngine::InitMono()
{
	mono_set_assemblies_path("mono/lib");

	spScriptEngineData = CreateRef<ScriptEngineData>();
	if (spScriptEngineData->EnableDebugging)
	{
		const char* argv[2] = {
			"--debugger-agent=transport=dt_socket,address=127.0.0.1:2550,server=y,suspend=n,loglevel=3,logfile=MonoDebugger.log",
			"--soft-breakpoints"
		};

		mono_jit_parse_options(2, (char**)argv);
		mono_debug_init(MONO_DEBUG_FORMAT_MONO);
	}
	spScriptEngineData->RootDomain = mono_jit_init("SandTableScriptRuntime");
	SAND_TABLE_ASSERT(spScriptEngineData->RootDomain, "RootDomain is null in InitMono");

	if (spScriptEngineData->EnableDebugging)
	{
		mono_debug_domain_create(spScriptEngineData->RootDomain);
	}

	mono_thread_set_main(mono_thread_current());
}

bool ScriptEngine::LoadAssemblyAndMonoImage()
{
	spScriptEngineData->AppDomain = mono_domain_create_appdomain("SandTableScriptRuntime", nullptr);
	SAND_TABLE_ASSERT(spScriptEngineData->AppDomain, "AppDomain is null in InitMono");
	mono_domain_set(spScriptEngineData->AppDomain, true);

	LoadAssembly("resources/script/SandTableScript.dll", spScriptEngineData->CoreAssembly, spScriptEngineData->CoreMonoImage);

	auto scriptModulePath = Project::GetProjectInstance()->GetAssetDirectory() /
		Project::GetProjectInstance()->GetProjectConfig()->ScriptModulePath;
	LoadAssembly(scriptModulePath, spScriptEngineData->AppAssembly, spScriptEngineData->AppMonoImage);
	if (spScriptEngineData->CoreAssembly==nullptr || spScriptEngineData->AppAssembly==nullptr)
	{
		LOG_DEV_ERROR("Load Assembly failed");
		return false;
	}

	spScriptEngineData->AssemblyReloadPending = false;
	spScriptEngineData->AppAssemblyFileWatcher = CreateScope<filewatch::FileWatch<std::string>>(
		scriptModulePath.string(), OnAppAssemblyFileSystemEvent);

	return true;
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
	if (iter != spScriptEngineData->ScriptEntityInstanceMap.end())
	{
		iter->second->InVokeOnUpdate(fTimeStep);
	}
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

MonoObject* ScriptEngine::GetMonoObject(const UUID& uiEntityID)
{
	auto iter = spScriptEngineData->ScriptEntityInstanceMap.find(uiEntityID);
	if (iter != spScriptEngineData->ScriptEntityInstanceMap.end())
	{
		return iter->second->GetMonoObject();
	}
	else
	{
		return nullptr;
	}
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

void ScriptEngine::ReloadAssembly()
{
	LOG_DEV_INFO("ReloadAssembly Begin");
	mono_domain_set(mono_get_root_domain(), false);

	mono_domain_unload(spScriptEngineData->AppDomain);

	if (!LoadAssemblyAndMonoImage())
	{
		return;
	}

	LoadAssemblyClass();

	ScriptGlue::RegisterComponents();
	LOG_DEV_INFO("ReloadAssembly End");
}

void ScriptEngine::LoadAssemblyClass()
{
	spScriptEngineData->CoreScriptEntityClass = CreateRef<ScriptEntityClass>("SandTable", "Entity",
		spScriptEngineData->CoreMonoImage, spScriptEngineData->AppDomain);
	MonoClass* pCoreClass = mono_class_from_name(spScriptEngineData->CoreMonoImage, "SandTable", "Entity");

	const auto& pMonoImage = spScriptEngineData->AppMonoImage;
	const MonoTableInfo* pClassNameType = mono_image_get_table_info(pMonoImage, MONO_TABLE_TYPEDEF);
	int iClassNum = mono_table_info_get_rows(pClassNameType);

	for (int i = 0; i < iClassNum; i++)
	{
		auto spTableInfo = CreateRef<DataBuffer>(MONO_TYPEDEF_SIZE, sizeof(uint32_t) / sizeof(uint8_t));
		mono_metadata_decode_row(pClassNameType, i, spTableInfo->As<uint32_t>(), MONO_TYPEDEF_SIZE);

		const char* pClassNameSpace = mono_metadata_string_heap(pMonoImage, spTableInfo->As<uint32_t>()[MONO_TYPEDEF_NAMESPACE]);
		const char* pClassName = mono_metadata_string_heap(pMonoImage, spTableInfo->As<uint32_t>()[MONO_TYPEDEF_NAME]);
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

MonoImage* ScriptEngine::GetCoreAssemblyImage()
{
	return spScriptEngineData->CoreMonoImage;
}

SAND_TABLE_NAMESPACE_END