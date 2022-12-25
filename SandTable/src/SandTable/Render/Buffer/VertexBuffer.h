#pragma once

#include "SandTable/Render/Buffer/Buffer.h"
#include "SandTable/Render/Buffer/VertexBufferLayout.h"

SAND_TABLE_NAMESPACE_BEGIN

class VertexBuffer :public Buffer
{
public:
	virtual ~VertexBuffer() = default;

	void SetVertexBufferLayout(const Ref<VertexBufferLayout>& spVertexBufferLayout);
	const Ref<VertexBufferLayout>& GetVertexBufferLayout()const;
	const std::vector<float>& GetVertex()const;

	virtual void SetData(const void* pData, uint32_t uiVertexSize) = 0;

	static Ref<Buffer> Create(const std::vector<float>& vecVertex, const Ref<VertexBufferLayout>& spVertexBufferLayout);
	static Ref<Buffer> Create(const std::vector<float>& vecVertex);
	static Ref<Buffer> Create(unsigned int uiVertexSize);

protected:
	VertexBuffer();
	VertexBuffer(const std::vector<float>& vecVertex);
	VertexBuffer(const std::vector<float>& vecVertex, const Ref<VertexBufferLayout>& spVertexBufferLayout);

private:
	VertexBuffer(VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;

protected:
	unsigned int m_uiRenderID;
	std::vector<float> m_vecVertex;
	Ref<VertexBufferLayout> m_spVertexBufferLayout;
};

SAND_TABLE_NAMESPACE_END