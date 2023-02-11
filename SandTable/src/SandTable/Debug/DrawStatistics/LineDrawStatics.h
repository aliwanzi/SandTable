#pragma once
#include "DrawStatistics.h"

SAND_TABLE_NAMESPACE_BEGIN
class LineDrawStatics:public DrawStatistics
{
public:
	// ͨ�� DrawStatistics �̳�
	virtual unsigned int GetTotalVertexCount() const override;
	virtual unsigned int GetTotalIndexCount() const override;
	virtual unsigned int GetMaxVertices() const override;
	virtual unsigned int GetMaxIndices() const override;
};
SAND_TABLE_NAMESPACE_END

