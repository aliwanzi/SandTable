#include "pch.h"
#include "DrawStatistics.h"

SAND_TABLE_NAMESPACE_BEGIN
DrawStatistics::DrawStatistics(unsigned int uiMaxDrawCount):
    m_uiMaxDrawCount(uiMaxDrawCount),
    m_uiDrawCount(0),
    m_uiDrawCalls(0)
{

}

unsigned int DrawStatistics::GetMaxDrawCount() const
{
    return m_uiMaxDrawCount;
}

unsigned int DrawStatistics::GetDrawCalls() const
{
    return m_uiDrawCalls;
}

unsigned int DrawStatistics::GetDrawCount() const
{
    return m_uiDrawCount;
}

void DrawStatistics::AddDrawCalls()
{
    m_uiDrawCalls++;
}

void DrawStatistics::AddDrawCount()
{
    m_uiDrawCount++;
}

void DrawStatistics::ResetDrawCalls()
{
    m_uiDrawCalls = 0;
}

void DrawStatistics::ResetDrawCount()
{
    m_uiDrawCount = 0;
}

SAND_TABLE_NAMESPACE_END