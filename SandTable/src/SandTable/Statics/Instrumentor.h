#pragma once
#include <chrono>
#include <iomanip>
#include <thread>
#include <mutex>

SAND_TABLE_NAMESPACE_BEGIN

using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

struct ProfileResult
{
	std::string Name;

	FloatingPointMicroseconds Start;
	std::chrono::microseconds ElapsedTime;
	std::thread::id ThreadID;
};

struct InstrumentationSession
{
	std::string Name;
};

class Instrumentor
{
public:
	Instrumentor(const Instrumentor&) = delete;
	Instrumentor& operator=(const Instrumentor&) = delete;
	~Instrumentor();
	void BeginSession(const std::string& sName, const std::string& sFilepath = "results.json");
	void EndSession();

	void WriteProfile(const ProfileResult& result);

	static Ref<Instrumentor> GetInstrumentor();
private:
	Instrumentor();
	void WriteHeader();
	void WriteFooter();

	void InternalEndSession();
private:
	std::mutex m_Mutex;
	std::ofstream m_OutputStream;
	Ref<InstrumentationSession> m_spCurrentSession;
	static Ref<Instrumentor> m_spInstrumentor;
};

class InstrumentationTimer
{
public:
	InstrumentationTimer(const std::string& sName);
	~InstrumentationTimer();

private:
	void Stop();
private:
	std::string m_sName;
	bool m_bStopped;
	std::chrono::time_point<std::chrono::steady_clock> m_spStartTimepoint;
};

SAND_TABLE_NAMESPACE_END

#define SAND_TABLE_PROFILE_BEGIN_SESSION(sName,sFilePath) ::SandTable::Instrumentor::GetInstrumentor()->BeginSession(sName,sFilePath);
#define SAND_TABLE_PROFILE_END_SESSION() ::SandTable::Instrumentor::GetInstrumentor()->EndSession();
#define SAND_TABLE_PROFILE_SCOPE(sName) ::SandTable::InstrumentationTimer timer##__LINE__(sName);
#define SAND_TABLE_PROFILE_FUNCTION() SAND_TABLE_PROFILE_SCOPE(__FUNCSIG__)
