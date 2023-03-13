#pragma once
#include "SandTable/Core/Application.h"

#ifdef SAND_TABLE_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	LOG_DEV_INFO("Entry Application!");

	SAND_TABLE_PROFILE_BEGIN_SESSION("Startup", "SandTableProfile-Startup.json");
	Application::CreateApplication(argc, argv);
	SAND_TABLE_PROFILE_END_SESSION();

	Application::GetApplication()->Run();

	LOG_DEV_INFO("Exit Application!");
	EXIT_SUCCESS;
}

#endif // ST_PLATFORM_WINDOWS
