#pragma once
#include "SandTable/Core/Core.h"
#include <string>
#include <chrono>

SAND_TABLE_NAMESPACE_BEGIN

template<typename Fn>
class Timer
{
public:
	Timer::Timer(const std::string& sName, Fn&& fun)
		:m_sName(sName), m_Fun(fun), m_bStopped(false)
	{
		m_spStartTimePoint = std::chrono::high_resolution_clock::now();
	}
	~Timer() 
	{
		if (!m_bStopped)
			Stop();
	};
private:
	void Stop()
	{
		auto spEndTimePoint = std::chrono::high_resolution_clock::now();
		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_spStartTimePoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(spEndTimePoint).time_since_epoch().count();
		m_bStopped = true;
		float fDuration = (end - start) * 0.001f;
		m_Fun({ m_sName,fDuration });
	}
private:
	std::string m_sName;
	std::chrono::time_point<std::chrono::steady_clock> m_spStartTimePoint;
	bool m_bStopped;
	Fn m_Fun;
};

SAND_TABLE_NAMESPACE_END

