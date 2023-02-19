#include "pch.h"
#include "ScriptEngine.h"
#include "ScriptGlue.h"
#include "ScriptClass.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"

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
	};
	static Ref<ScriptEngineData> spScriptEngineData;
}

void ScriptEngine::Init()
{
	InitMono();
	LoadAssembly("Resources/Scripts/SandTable-ScriptCore.dll");

	ScriptGlue::RegisterFunctions();
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
	size_t iFileSize = end - stream.tellg();
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

SAND_TABLE_NAMESPACE_END