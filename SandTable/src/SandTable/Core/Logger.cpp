#include "pch.h"
#include "Logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

SAND_TABLE_NAMESPACE_BEGIN

Ref<spdlog::logger> Logger::m_spLogger = nullptr;
Ref<spdlog::logger> Logger::GetLogger()
{
	if (!m_spLogger)
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(CreateRef<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(CreateRef<spdlog::sinks::basic_file_sink_mt>("SandTable.log", true));

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		m_spLogger = CreateRef<spdlog::logger>("SandTable", begin(logSinks), end(logSinks));
		spdlog::register_logger(m_spLogger);
		m_spLogger->set_level(spdlog::level::trace);
		m_spLogger->flush_on(spdlog::level::trace);
	}
	return m_spLogger;
}

SAND_TABLE_NAMESPACE_END