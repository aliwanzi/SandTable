#include "pch.h"
#include "SandTable/Render/RenderAPI.h"
#include "PlatForm/OpenGL/Buffer/OpenGLVertexBuffer.h"

SAND_TABLE_NAMESPACE_BEGIN

const std::vector<float>& VertexBuffer::GetVertex() const
{
	return m_vecVertex;
}

const Ref<VertexBufferLayout>& VertexBuffer::GetVertexBufferLayout() const
{
	return m_spVertexBufferLayout;
}

void VertexBuffer::SetVertexBufferLayout(const Ref<VertexBufferLayout>& spVertexBufferLayout)
{
	m_spVertexBufferLayout = spVertexBufferLayout;
}

Ref<Buffer> VertexBuffer::Create(const std::vector<float>& vecVertex,
	const Ref<VertexBufferLayout>& spVertexBufferLayout)
{
	switch (RenderAPI::GetAPIType())
	{
	case RenderAPI::APIType::OpenGL:
		return CreateRef<OpenGLVertexBuffer>(vecVertex, spVertexBufferLayout);
	default:
		SAND_TABLE_ASSERT(false, "Unknown RenderAPI");
		return nullptr;
	}
}

Ref<Buffer> VertexBuffer::Create(const std::vector<float>& vecVertex)
{
	switch (RenderAPI::GetAPIType())
	{
	case RenderAPI::APIType::OpenGL:
		return CreateRef<OpenGLVertexBuffer>(vecVertex);
	default:
		SAND_TABLE_ASSERT(false, "Unknown RenderAPI");
		return nullptr;
	}
}

Ref<Buffer> VertexBuffer::Create(unsigned int uiVertexSize)
{
	switch (RenderAPI::GetAPIType())
	{
	case RenderAPI::APIType::OpenGL:
		return CreateRef<OpenGLVertexBuffer>(uiVertexSize);
	default:
		SAND_TABLE_ASSERT(false, "Unknown RenderAPI");
		return nullptr;
	}
}

VertexBuffer::VertexBuffer(const std::vector<float>& vecVertex,
	const Ref<VertexBufferLayout>& spVertexBufferLayout):
	m_vecVertex(vecVertex),
	m_spVertexBufferLayout(spVertexBufferLayout)
{

}

VertexBuffer::VertexBuffer():m_spVertexBufferLayout(nullptr)
{

}

VertexBuffer::VertexBuffer(const std::vector<float>& vecVertex):
	m_vecVertex(vecVertex),
	m_spVertexBufferLayout(nullptr)
{

}


SAND_TABLE_NAMESPACE_END