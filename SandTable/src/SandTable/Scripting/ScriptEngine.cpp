#include "pch.h"
#include "ScriptEngine.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

SAND_TABLE_NAMESPACE_BEGIN

namespace
{
	struct ScriptEngineData
	{
		Ref<MonoDomain> RootDomain = nullptr;
		Ref<MonoDomain> AppDomain = nullptr;
		Ref<MonoAssembly> CoreAssembly = nullptr;

		~ScriptEngineData()
		{
			//mono_domain_set(mono_get_root_domain(), false);
			//mono_domain_unload(AppDomain.get());
			//mono_jit_cleanup(RootDomain.get());
		}
	};
	static Ref<ScriptEngineData> spScriptEngineData;

	Ref<MonoAssembly> LoadCSharpAssembly(const std::string& sAssemblyPath)
	{
		std::ifstream stream(sAssemblyPath, std::ios::binary | std::ios::ate);
		if (!stream)
		{
			LOG_DEV_ERROR("Read Assembly {0} Failed", sAssemblyPath);
			return false;
		}

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		size_t iFileSize = end - stream.tellg();
		if (iFileSize == 0)
		{
			LOG_DEV_ERROR("Read Assembly {0} is Null", sAssemblyPath);
			return false;
		}

		auto spFileData = Ref<char>(new char[iFileSize], std::default_delete<char>());
		stream.read(spFileData.get(), iFileSize);
		stream.close();

		MonoImageOpenStatus status;
		MonoImage* pMonoImage = mono_image_open_from_data_full(spFileData.get(), iFileSize, true, &status, false);
		if (status != MONO_IMAGE_OK)
		{
			LOG_DEV_ERROR(mono_image_strerror(status));
		}

		auto spAssembly = Ref<MonoAssembly>(mono_assembly_load_from_full(pMonoImage, sAssemblyPath.c_str(), &status, false));
		mono_image_close(pMonoImage);
		if (status != MONO_IMAGE_OK)
		{
			LOG_DEV_ERROR(mono_image_strerror(status));
		}

		return spAssembly;
	}

	void PrintAssemblyTypes(MonoImage* pMonoImage)
	{
		const MonoTableInfo* pTypeDefinitionsTable = mono_image_get_table_info(pMonoImage, MONO_TABLE_TYPEDEF);
		int iNumTypes = mono_table_info_get_rows(pTypeDefinitionsTable);

		for (int i = 0; i < iNumTypes; i++)
		{
			auto spCols = Ref<uint32_t>(new uint32_t[MONO_TYPEDEF_SIZE], std::default_delete<uint32_t>());
			mono_metadata_decode_row(pTypeDefinitionsTable, i, spCols.get(), MONO_TYPEDEF_SIZE);
			const char* pNameSpace = mono_metadata_string_heap(pMonoImage, spCols.get()[MONO_TYPEDEF_NAMESPACE]);
			const char* pName = mono_metadata_string_heap(pMonoImage, spCols.get()[MONO_TYPEDEF_NAME]);

			LOG_DEV_INFO("{}.{}", pNameSpace, pName);
		}

	}

	void CppFunc()
	{
		std::cout << "This is written in C++!" << std::endl;
	}

	void NativeLogInt(MonoString* pStrLog, int iPar)
	{
		char* pLog = mono_string_to_utf8(pStrLog);
		std::cout << pLog << " " << iPar << std::endl;
		mono_free(pLog);
	}

	void NativeLogVec3(glm::vec3* vec3Par, glm::vec3* vec3Out)
	{
		LOG_DEV_INFO("pStrLog Value:{0}", *vec3Par);
		*vec3Out = glm::normalize(*vec3Par);
	}

	float NativeLogVec3Dot(glm::vec3* vec3Par)
	{
		LOG_DEV_INFO("pStrLog Value:{0}", *vec3Par);
		return glm::dot(*vec3Par, *vec3Par);
	}
}

void ScriptEngine::Init()
{
	spScriptEngineData = CreateRef<ScriptEngineData>();
	InitMono();
}

void ScriptEngine::InitMono()
{
	mono_set_assemblies_path("mono/lib");

	spScriptEngineData->RootDomain = Ref<MonoDomain>(mono_jit_init("SandTableScriptRuntime"));
	SAND_TABLE_ASSERT(spScriptEngineData->RootDomain, "RootDomain is null in InitMono");

	spScriptEngineData->AppDomain = Ref<MonoDomain>(mono_domain_create_appdomain("SandTableScriptRuntime", nullptr));
	mono_domain_set(spScriptEngineData->AppDomain.get(), true);

	mono_add_internal_call("SandTable.Main::CppFunction", CppFunc);

	mono_add_internal_call("SandTable.Main::NativeLogInt", NativeLogInt);
	mono_add_internal_call("SandTable.Main::NativeLogVec3", NativeLogVec3);
	mono_add_internal_call("SandTable.Main::NativeLogVec3Dot", NativeLogVec3Dot);

	spScriptEngineData->CoreAssembly = LoadCSharpAssembly("Resources/Scripts/SandTable-ScriptCore.dll");

	//1. create an object (and call constructor)
	MonoImage* pMonoImage = mono_assembly_get_image(spScriptEngineData->CoreAssembly.get());
	MonoClass* pMonoClass = mono_class_from_name(pMonoImage, "SandTable", "Main");
	MonoObject* pMonoObject = mono_object_new(spScriptEngineData->AppDomain.get(), pMonoClass);
	SAND_TABLE_ASSERT(pMonoObject, "Mono Object is null in ScriptEngine");
	mono_runtime_object_init(pMonoObject);

	//2. call function
	MonoMethod* pPrintMessageFunc = mono_class_get_method_from_name(pMonoClass, "PrintMessage", 0);
	mono_runtime_invoke(pPrintMessageFunc, pMonoObject, nullptr, nullptr);

	MonoMethod* pPrintIntFunc= mono_class_get_method_from_name(pMonoClass, "PrintInt", 1);
	int iValue1(5);
	void* pParam = &iValue1;
	mono_runtime_invoke(pPrintIntFunc, pMonoObject, &pParam, nullptr);

	MonoMethod* pPrintIntsFunc = mono_class_get_method_from_name(pMonoClass, "PrintInts", 2);
	int iValue2(10);
	void* pParams[2] = {
		&iValue1,
		&iValue2
	};
	mono_runtime_invoke(pPrintIntsFunc, pMonoObject, pParams, nullptr);


	MonoString* pMonoString = mono_string_new(spScriptEngineData->AppDomain.get(), "Hello World from c++");
	MonoMethod* pPrintCustomMessageFunc = mono_class_get_method_from_name(pMonoClass, "PrintCustomMessage", 1);
	void* sParam = pMonoString;
	mono_runtime_invoke(pPrintCustomMessageFunc, pMonoObject, &sParam, nullptr);


	//SAND_TABLE_ASSERT(false,"Error");
}

SAND_TABLE_NAMESPACE_END