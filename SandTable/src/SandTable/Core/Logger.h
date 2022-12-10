#pragma once
#include "Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace SandTable
{
	class SAND_TABLE_API Logger
	{
	public:
		static void Init();
		static std::shared_ptr<spdlog::logger>& GetLogger();
	private:
		static std::shared_ptr<spdlog::logger> m_spLogger;
	};
}

#define LOG_DEV_ERROR(...)	::SandTable::Logger::GetLogger()->error(__VA_ARGS__)
#define LOG_DEV_WARN(...)	::SandTable::Logger::GetLogger()->warn(__VA_ARGS__)
#define LOG_DEV_INFO(...)	::SandTable::Logger::GetLogger()->info(__VA_ARGS__)
#define LOG_DEV_TRACE(...)	::SandTable::Logger::GetLogger()->trace(__VA_ARGS__)
