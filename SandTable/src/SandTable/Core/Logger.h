#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

SAND_TABLE_NAMESPACE_BEGIN

class Logger
{
public:
	static Ref<spdlog::logger> GetLogger();
private:
	Logger() = delete;
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

	static Ref<spdlog::logger> m_spLogger;
};

SAND_TABLE_NAMESPACE_END

#define LOG_DEV_ERROR(...)	::SandTable::Logger::GetLogger()->error(__VA_ARGS__)
#define LOG_DEV_WARN(...)	::SandTable::Logger::GetLogger()->warn(__VA_ARGS__)
#define LOG_DEV_INFO(...)	::SandTable::Logger::GetLogger()->info(__VA_ARGS__)
#define LOG_DEV_TRACE(...)	::SandTable::Logger::GetLogger()->trace(__VA_ARGS__)
