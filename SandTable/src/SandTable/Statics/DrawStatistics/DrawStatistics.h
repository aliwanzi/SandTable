#pragma once

SAND_TABLE_NAMESPACE_BEGIN
class DrawStatistics
{
public:
	DrawStatistics(unsigned int uiMaxDrawCount);
	virtual ~DrawStatistics() = default;
	virtual unsigned int GetTotalVertexCount()const = 0;
	virtual unsigned int GetTotalIndexCount()const = 0;
	virtual unsigned int GetMaxVertices()const = 0;
	virtual unsigned int GetMaxIndices()const = 0;
	unsigned int GetMaxDrawCount()const;
	unsigned int GetDrawCalls() const;
	unsigned int GetDrawCount()const;

	void AddDrawCalls();
	void AddDrawCount();
	void ResetDrawCalls();
	void ResetDrawCount();
protected:
	unsigned int m_uiDrawCalls;
	unsigned int m_uiDrawCount;
	unsigned int m_uiMaxDrawCount;
};

SAND_TABLE_NAMESPACE_END

