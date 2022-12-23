#include "pch.h"
#include "Instrumentor.h"

SAND_TABLE_NAMESPACE_BEGIN
Ref<Instrumentor> Instrumentor::m_spInstrumentor = nullptr;

Instrumentor::Instrumentor() : m_spCurrentSession(nullptr)
{

}

void Instrumentor::BeginSession(const std::string& sName, const std::string& sFilepath)
{
	std::lock_guard lock(m_Mutex);
	if (m_spCurrentSession)
	{
		LOG_DEV_WARN("Instrumentor::BeginSession('{0}') when session '{1}' already open.", sName, m_spCurrentSession->Name);
		InternalEndSession();
	}

	m_OutputStream.open(sFilepath);

	if (m_OutputStream.is_open())
	{
		m_spCurrentSession = Ref<InstrumentationSession>(new InstrumentationSession({ sName }));
		WriteHeader();
	}
	else
	{
		LOG_DEV_ERROR("Instrumentor could not open results file '{0}'.", sFilepath);
	}
}

void Instrumentor::EndSession()
{
	std::lock_guard lock(m_Mutex);
	InternalEndSession();
}

void Instrumentor::WriteProfile(const ProfileResult& result)
{
	std::stringstream json;

	json << std::setprecision(3) << std::fixed;
	json << ",{";
	json << "\"cat\":\"function\",";
	json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
	json << "\"name\":\"" << result.Name << "\",";
	json << "\"ph\":\"X\",";
	json << "\"pid\":0,";
	json << "\"tid\":" << result.ThreadID << ",";
	json << "\"ts\":" << result.Start.count();
	json << "}";

	std::lock_guard lock(m_Mutex);
	if (m_spCurrentSession)
	{
		m_OutputStream << json.str();
		m_OutputStream.flush();
	}
}

Ref<Instrumentor> Instrumentor::GetInstrumentor()
{
	if (m_spInstrumentor == nullptr)
	{
		m_spInstrumentor = Ref<Instrumentor>(new Instrumentor());
	}
	return m_spInstrumentor;
}

Instrumentor::~Instrumentor()
{
	EndSession();
}

void Instrumentor::WriteHeader()
{
	m_OutputStream << "{\"otherData\": {},\"traceEvents\":[{}";
	m_OutputStream.flush();
}

void Instrumentor::WriteFooter()
{
	m_OutputStream << "]}";
	m_OutputStream.flush();
}

void Instrumentor::InternalEndSession()
{
	if (m_spCurrentSession)
	{
		WriteFooter();
		m_OutputStream.close();
	}
}

InstrumentationTimer::InstrumentationTimer(const std::string& sName):m_sName(sName),m_bStopped(false)
{
	m_spStartTimepoint = std::chrono::steady_clock::now();
}

InstrumentationTimer::~InstrumentationTimer()
{
	if (!m_bStopped)
		Stop();
}

void InstrumentationTimer::Stop()
{
	auto spEndTimepoint = std::chrono::steady_clock::now();
	auto highResStart = FloatingPointMicroseconds{ m_spStartTimepoint.time_since_epoch() };
	auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(spEndTimepoint).time_since_epoch() - 
		std::chrono::time_point_cast<std::chrono::microseconds>(m_spStartTimepoint).time_since_epoch();

	Instrumentor::GetInstrumentor()->WriteProfile({m_sName, highResStart, elapsedTime, std::this_thread::get_id() });

	m_bStopped = true;
}

SAND_TABLE_NAMESPACE_END


