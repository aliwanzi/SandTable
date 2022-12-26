#include "pch.h"
#include "Random.h"
#include <limits>

SAND_TABLE_NAMESPACE_BEGIN

std::mt19937 Random::m_RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::m_Distribution;

void Random::Init()
{
	m_RandomEngine.seed(std::random_device()());
}

float Random::Float()
{
	return static_cast<float>(m_Distribution(m_RandomEngine)) / static_cast<float>(UINT_MAX);
}

SAND_TABLE_NAMESPACE_END


