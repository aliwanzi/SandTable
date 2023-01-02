#pragma once
#include "SandTable/Core/Application.h"

#ifdef SAND_TABLE_PLATFORM_WINDOWS

int main()
{
	LOG_DEV_WARN("Initialize Log!");

	SAND_TABLE_PROFILE_BEGIN_SESSION("Startup", "SandTableProfile-Startup.json");
	SandTable::Application::CreateApplication();
	SAND_TABLE_PROFILE_END_SESSION();

	SAND_TABLE_PROFILE_BEGIN_SESSION("Runtime", "SandTableProfile-Runtime.json");
	auto spApplication = SandTable::Application::GetApplication();
	spApplication->Run();
	SAND_TABLE_PROFILE_END_SESSION();

	EXIT_SUCCESS;
}

#endif // ST_PLATFORM_WINDOWS
