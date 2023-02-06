#include "pch.h"
#include "CircleDrawStatics.h"

SAND_TABLE_NAMESPACE_BEGIN

CircleDrawStatics::CircleDrawStatics(unsigned int uiMaxDrawCount):
	DrawStatistics(uiMaxDrawCount)
{
}

unsigned int CircleDrawStatics::GetTotalVertexCount() const
{
	return m_uiDrawCount * 4;
}

unsigned int CircleDrawStatics::GetTotalIndexCount() const
{
	return m_uiDrawCount * 6;
}

unsigned int CircleDrawStatics::GetMaxVertices() const
{
	return m_uiMaxDrawCount * 4;
}

unsigned int CircleDrawStatics::GetMaxIndices() const
{
	return m_uiMaxDrawCount * 6;
}

SAND_TABLE_NAMESPACE_END
