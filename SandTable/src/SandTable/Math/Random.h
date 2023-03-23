#pragma once
#include <random>

SAND_TABLE_NAMESPACE_BEGIN

class Random
{
public:
	static float Float();

	static int Uint(int min, int max);
	static uint32_t Uint32();
	static uint64_t Uint64();

	static glm::dvec3 Vec3();

	static glm::dvec3 UnitSphere();
	static glm::dvec3 UnitDisk();
private:
	static thread_local std::mt19937 m_sRandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> m_sDistribution;
};

SAND_TABLE_NAMESPACE_END