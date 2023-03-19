#include "pch.h"
#include "Random.h"
#include <limits>

SAND_TABLE_NAMESPACE_BEGIN

thread_local std::mt19937 Random::m_sRandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::m_sDistribution;


float Random::Float()
{
	return static_cast<float>(m_sDistribution(m_sRandomEngine)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
}

uint32_t Random::Uint32()
{
	return static_cast<uint32_t>(m_sDistribution(m_sRandomEngine));
}

uint64_t Random::Uint64()
{
	return static_cast<uint64_t>(m_sDistribution(m_sRandomEngine));
}

glm::vec3 Random::Vec3()
{
	return glm::vec3(Float(), Float(), Float());
}



SAND_TABLE_NAMESPACE_END


