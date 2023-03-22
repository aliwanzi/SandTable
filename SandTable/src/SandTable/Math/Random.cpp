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

glm::vec3 Random::UnitSphere()
{
	while (true)
	{
		auto unit = glm::vec3(2 * Float() - 1, 2 * Float() - 1, 2 * Float() - 1);
		if (glm::length(unit) >= 1.f)
			continue;
		return unit;
	}
}

glm::vec3 Random::UnitDisk()
{
	while (true)
	{
		auto unit = glm::vec3(2 * Float() - 1, 2 * Float() - 1, 0);
		if (glm::length(unit) >= 1.f || abs(glm::length(unit) < glm::epsilon<float>()))
			continue;
		return unit;
	}
}



SAND_TABLE_NAMESPACE_END


