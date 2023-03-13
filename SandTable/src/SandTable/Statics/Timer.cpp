#include "pch.h"
#include "Timer.h"
SAND_TABLE_NAMESPACE_BEGIN
Timer::Timer()
{
	Reset();
}

void Timer::Reset()
{
	m_sTimePoint = std::chrono::high_resolution_clock::now();
}

float Timer::Elapsed()
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_sTimePoint).count() 
		* 0.001f * 0.001f * 0.001f;
}

float Timer::ElapsedMillis()
{
	return Elapsed() * 1000.0f;
}
SAND_TABLE_NAMESPACE_END