#include "pch.h"
#include "SandTable/Render/RenderAPI.h"
#include "PlatForm/OpenGL/Buffer/OpenGLVertexBuffer.h"

namespace SandTable
{
	const std::vector<float>& VertexBuffer::GetVertex() const
	{
		return m_vecVertex;
	}

	const std::shared_ptr<VertexBufferLayout>& VertexBuffer::GetVertexBufferLayout() const
	{
		return m_spVertexBufferLayout;
	}

	std::shared_ptr<Buffer> VertexBuffer::Create(const std::vector<float>& vecVertex,
		const std::shared_ptr<VertexBufferLayout>& spVertexBufferLayout)
	{
		switch (RenderAPI::GetAPIType())
		{
		case RenderAPI::APIType::OpenGL:
			return std::shared_ptr<OpenGLVertexBuffer>(new OpenGLVertexBuffer(vecVertex, spVertexBufferLayout));
		default:
			SAND_TABLE_ASSERT(false, "Unknown RenderAPI");
			return nullptr;
		}
	}

	VertexBuffer::VertexBuffer(const std::vector<float>& vecVertex,
		const std::shared_ptr<VertexBufferLayout>& spVertexBufferLayout) :Buffer(),
		m_vecVertex(vecVertex),
		m_spVertexBufferLayout(spVertexBufferLayout)
	{

	}
}

