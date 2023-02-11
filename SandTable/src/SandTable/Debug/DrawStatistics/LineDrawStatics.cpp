#include "pch.h"
#include "LineDrawStatics.h"

SAND_TABLE_NAMESPACE_BEGIN
LineDrawStatics::LineDrawStatics(unsigned int uiMaxDrawCount) :
	DrawStatistics(uiMaxDrawCount)
{

}

unsigned int LineDrawStatics::GetTotalVertexCount() const
{
    return m_uiDrawCount * 2;
}

unsigned int LineDrawStatics::GetTotalIndexCount() const
{
    return m_uiDrawCount * 2;
}

unsigned int LineDrawStatics::GetMaxVertices() const
{
    return m_uiMaxDrawCount * 2;
}

unsigned int LineDrawStatics::GetMaxIndices() const
{
    return m_uiMaxDrawCount * 2;
}

SAND_TABLE_NAMESPACE_END