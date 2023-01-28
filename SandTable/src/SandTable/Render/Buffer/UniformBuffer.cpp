#include "pch.h"
#include "SandTable/Render/RenderAPI.h"
#include "PlatForm/OpenGL/Buffer/OpenGLUniformBuffer.h"

SAND_TABLE_NAMESPACE_BEGIN

Ref<Buffer> UniformBuffer::Create(unsigned int uiSize, unsigned int uiBinding)
{
	switch (RenderAPI::GetAPIType())
	{
	case RenderAPI::APIType::OpenGL:
		return CreateRef<OpenGLUniformBuffer>(uiSize, uiBinding);
	default:
		SAND_TABLE_ASSERT(false, "Unknown RenderAPI");
		return nullptr;
	}
}

SAND_TABLE_NAMESPACE_END
