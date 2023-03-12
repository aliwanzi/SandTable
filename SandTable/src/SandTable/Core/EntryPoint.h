#pragma once
#include "SandTable/Core/Application.h"

#ifdef SAND_TABLE_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	using namespace SandTable;
	LOG_DEV_WARN("Initialize Log!");

	auto spApplicationSpecification = CreateRef<ApplicationSpecification>();
	spApplicationSpecification->Name = "SandBox";
	spApplicationSpecification->CommandLineArgs = { argc, argv };
	auto spApplication = SandTable::Application::GetApplication(spApplicationSpecification);

	SAND_TABLE_PROFILE_BEGIN_SESSION("Startup", "SandTableProfile-Startup.json");
	Application::CreateApplication();
	SAND_TABLE_PROFILE_END_SESSION();

	SAND_TABLE_PROFILE_BEGIN_SESSION("Runtime", "SandTableProfile-Runtime.json");
	spApplication->Run();
	SAND_TABLE_PROFILE_END_SESSION();

	EXIT_SUCCESS;
}

#endif // ST_PLATFORM_WINDOWS
