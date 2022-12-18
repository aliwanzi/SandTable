#include "pch.h"
#include "PlatForm/OpenGL/OpenGLVertexArray.h"
#include "SandTable/Render/RenderAPI.h"

namespace SandTable
{

	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		switch (RenderAPI::GetAPIType())
		{
		case RenderAPI::APIType::OpenGL:
			return std::shared_ptr<OpenGLVertexArray>(new OpenGLVertexArray());
		default:
			SAND_TABLE_ASSERT(false, "Unknown RenderAPI");
			return nullptr;
		}
	}

	VertexArray::VertexArray():m_uiRenderID(0)
	{
	}
}
