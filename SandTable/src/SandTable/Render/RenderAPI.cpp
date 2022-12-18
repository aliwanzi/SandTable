#include "pch.h"
#include "PlatForm/OpenGL/OpenGLRenderAPI.h"

namespace SandTable
{
	RenderAPI::APIType RenderAPI::GetAPIType()
	{
#ifdef SAND_TABLE_RENDER_OPENGL
		return APIType::OpenGL;
#else
		return APIType::None;
#endif
	}

	std::shared_ptr<RenderAPI> RenderAPI::CreateRenderAPI()
	{
		switch (GetAPIType())
		{
			case APIType::OpenGL:
				return std::make_shared<OpenGLRenderAPI>();
			default:
			{
				SAND_TABLE_ASSERT(false, "Unknown API Type");
				return nullptr;
			}
		}
	}
}
