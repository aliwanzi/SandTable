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
		const auto& pMonoImage = mono_image_open_from_data_full(spFileData.get(), iFileSize, true, &status, false);
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

	void PrintAssemblyTypes(Ref<MonoAssembly> spAssembly)
	{
		const auto& pMonoImage = mono_assembly_get_image(spAssembly.get());
		const auto& pTypeDefinitionsTable = mono_image_get_table_info(pMonoImage, MONO_TABLE_TYPEDEF);
		int iNumTypes = mono_table_info_get_rows(pTypeDefinitionsTable);

		for (int i = 0; i < iNumTypes; i++)
		{
			auto spCols = Ref<uint32_t>(new uint32_t[MONO_TYPEDEF_SIZE], std::default_delete<uint32_t>());
			mono_metadata_decode_row(pTypeDefinitionsTable, i, spCols.get(), MONO_TYPEDEF_SIZE);
			const auto& pNameSpace = mono_metadata_string_heap(pMonoImage, spCols.get()[MONO_TYPEDEF_NAMESPACE]);
			const auto& pName = mono_metadata_string_heap(pMonoImage, spCols.get()[MONO_TYPEDEF_NAME]);

			LOG_DEV_INFO("{}.{}", pNameSpace, pName);
		}

	}
}

void ScriptEngine::Init()
{
	spScriptEngineData = CreateRef<ScriptEngineData>();
	InitMono();
}

void ScriptEngine::ShutDown()
{
	ShutDownMono();
}

void ScriptEngine::InitMono()
{
	mono_set_assemblies_path("mono/lib");

	spScriptEngineData->RootDomain = Ref<MonoDomain>(mono_jit_init("SandTableScriptRuntime"));
	SAND_TABLE_ASSERT(spScriptEngineData->RootDomain, "RootDomain is null in InitMono");

	spScriptEngineData->AppDomain = Ref<MonoDomain>(mono_domain_create_appdomain("SandTableScriptRuntime", nullptr));
	mono_domain_set(spScriptEngineData->AppDomain.get(), true);

	spScriptEngineData->CoreAssembly = LoadCSharpAssembly("Resources/Scripts/SandTable-ScriptCore.dll");
	PrintAssemblyTypes(spScriptEngineData->CoreAssembly);
}

void ScriptEngine::ShutDownMono()
{

}

SAND_TABLE_NAMESPACE_END