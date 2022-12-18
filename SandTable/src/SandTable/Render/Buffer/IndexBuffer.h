#pragma once
#include "SandTable/Render/Buffer/Buffer.h"

SAND_TABLE_NAMESPACE_BEGIN

class IndexBuffer :public Buffer
{
public:
	~IndexBuffer() = default;
	static Ref<Buffer> Create(const std::vector<int>& vecIndex);
	const std::vector<int> GetIndex()const;

private:
	IndexBuffer() = delete;
	IndexBuffer(IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;

protected:
	IndexBuffer(const std::vector<int>& vecIndex);

protected:
	std::vector<int> m_vecIndex;
};

SAND_TABLE_NAMESPACE_END