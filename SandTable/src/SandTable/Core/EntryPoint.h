#pragma once
#include "Application.h"
#include "Logger.h"

#ifdef SAND_TABLE_PLATFORM_WINDOWS

int main()
{
	LOG_DEV_WARN("Initialize Log!");
	SandTable::Application::CreateApplication();
	auto spApplication = SandTable::Application::GetApplication();
	spApplication->Run();
}

#endif // ST_PLATFORM_WINDOWS
