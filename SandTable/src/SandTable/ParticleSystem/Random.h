#pragma once
#include "SandTable/Core/Core.h"
#include <random>

SAND_TABLE_NAMESPACE_BEGIN

class Random
{
public:
	static void Init();
	static float Float();
private:
	static std::mt19937 m_RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> m_Distribution;
};

SAND_TABLE_NAMESPACE_END