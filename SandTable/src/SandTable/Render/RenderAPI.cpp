#include "pch.h"
#include "PlatForm/OpenGL/OpenGLRenderAPI.h"

SAND_TABLE_NAMESPACE_BEGIN

RenderAPI::APIType RenderAPI::GetAPIType()
{
#ifdef SAND_TABLE_RENDER_OPENGL
	return APIType::OpenGL;
#else
	return APIType::None;
#endif
}

Ref<RenderAPI> RenderAPI::CreateRenderAPI()
{
	switch (GetAPIType())
	{
	case APIType::OpenGL:
		return CreateRef<OpenGLRenderAPI>();
	default:
	{
		SAND_TABLE_ASSERT(false, "Unknown API Type");
		return nullptr;
	}
	}
}

SAND_TABLE_NAMESPACE_END
