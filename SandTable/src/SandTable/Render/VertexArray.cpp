#include "pch.h"
#include "PlatForm/OpenGL/OpenGLVertexArray.h"
#include "SandTable/Render/RenderAPI.h"
#include "SandTable/Statics/Instrumentor.h"

SAND_TABLE_NAMESPACE_BEGIN

Ref<VertexArray> VertexArray::Create()
{
	SAND_TABLE_PROFILE_FUNCTION();
	switch (RenderAPI::GetAPIType())
	{
	case RenderAPI::APIType::OpenGL:
		return CreateRef<OpenGLVertexArray>();
	default:
		SAND_TABLE_ASSERT(false, "Unknown RenderAPI");
		return nullptr;
	}
}

VertexArray::VertexArray() :m_uiRenderID(0)
{
}

SAND_TABLE_NAMESPACE_END