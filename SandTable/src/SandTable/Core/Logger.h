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

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

#define LOG_DEV_ERROR(...)	::SandTable::Logger::GetLogger()->error(__VA_ARGS__)
#define LOG_DEV_WARN(...)	::SandTable::Logger::GetLogger()->warn(__VA_ARGS__)
#define LOG_DEV_INFO(...)	::SandTable::Logger::GetLogger()->info(__VA_ARGS__)
#define LOG_DEV_TRACE(...)	::SandTable::Logger::GetLogger()->trace(__VA_ARGS__)
