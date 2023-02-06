#pragma once
#include "DrawStatistics.h"

SAND_TABLE_NAMESPACE_BEGIN

class CircleDrawStatics:public DrawStatistics
{
public:
	CircleDrawStatics(unsigned int uiMaxDrawCount);
	// Í¨¹ý DrawStatistics ¼Ì³Ð
	virtual unsigned int GetTotalVertexCount() const override;
	virtual unsigned int GetTotalIndexCount() const override;
	virtual unsigned int GetMaxVertices() const override;
	virtual unsigned int GetMaxIndices() const override;
};

SAND_TABLE_NAMESPACE_END



