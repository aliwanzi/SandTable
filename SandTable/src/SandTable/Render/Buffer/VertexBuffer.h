#pragma once

#include "SandTable/Render/Buffer/Buffer.h"
#include "SandTable/Render/Buffer/VertexBufferLayout.h"

SAND_TABLE_NAMESPACE_BEGIN

class VertexBuffer :public Buffer
{
public:
	virtual ~VertexBuffer() = default;
	const std::vector<float>& GetVertex()const;
	const Ref<VertexBufferLayout>& GetVertexBufferLayout()const;

	static Ref<Buffer> Create(const std::vector<float>& vecVertex,
		const Ref<VertexBufferLayout>& spVertexBufferLayout);

protected:
	VertexBuffer(const std::vector<float>& vecVertex,
		const Ref<VertexBufferLayout>& spVertexBufferLayout);
private:
	VertexBuffer() = delete;
	VertexBuffer(VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;

protected:
	unsigned int m_uiRenderID;
	std::vector<float> m_vecVertex;
	Ref<VertexBufferLayout> m_spVertexBufferLayout;
};

SAND_TABLE_NAMESPACE_END