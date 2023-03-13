#pragma once
#include <chrono>

SAND_TABLE_NAMESPACE_BEGIN
class Timer
{
public:
	Timer();
	void Reset();
	float Elapsed();
	float ElapsedMillis();
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_sTimePoint;
};

SAND_TABLE_NAMESPACE_END