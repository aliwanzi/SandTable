#include "pch.h"
#include "SandTable/Render/RenderAPI.h"
#include "PlatForm/OpenGL/Buffer/OpenGLIndexBuffer.h"

namespace SandTable
{
	std::shared_ptr<Buffer> IndexBuffer::Create(const std::vector<int>& vecIndex)
	{
		switch (RenderAPI::GetAPIType())
		{
		case RenderAPI::APIType::OpenGL:
			return std::shared_ptr<OpenGLIndexBuffer>(new OpenGLIndexBuffer(vecIndex));
		default:
			SAND_TABLE_ASSERT(false, "Unknown RenderAPI");
			return nullptr;
		}
	}

	const std::vector<int> IndexBuffer::GetIndex() const
	{
		return m_vecIndex;
	}

	IndexBuffer::IndexBuffer(const std::vector<int>& vecIndex):m_vecIndex(vecIndex)
	{

	}


}
