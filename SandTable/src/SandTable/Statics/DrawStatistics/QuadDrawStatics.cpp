#include "pch.h"
#include "QuadDrawStatics.h"

SAND_TABLE_NAMESPACE_BEGIN
QuadDrawStatics::QuadDrawStatics(unsigned int uiMaxDrawCount) :
	DrawStatistics(uiMaxDrawCount)
{

}

unsigned int QuadDrawStatics::GetTotalVertexCount() const
{
	return m_uiDrawCount * 4;
}

unsigned int QuadDrawStatics::GetTotalIndexCount() const
{
	return m_uiDrawCount * 6;
}

unsigned int QuadDrawStatics::GetMaxVertices() const
{
	return m_uiMaxDrawCount * 4;
}

unsigned int QuadDrawStatics::GetMaxIndices() const
{
	return m_uiMaxDrawCount * 6;
}
SAND_TABLE_NAMESPACE_END