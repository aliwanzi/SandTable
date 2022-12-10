#pragma once
#include "Application.h"
#include "Logger.h"

#ifdef SAND_TABLE_PLATFORM_WINDOWS

int main()
{
	SandTable::Logger::Init();
	LOG_DEV_WARN("Initialize Log!");
	auto spApplication = SandTable::Application::CreateApplication();
	spApplication->Run();
}

#endif // ST_PLATFORM_WINDOWS
