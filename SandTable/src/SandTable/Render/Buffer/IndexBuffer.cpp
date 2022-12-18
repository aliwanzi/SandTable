#include "pch.h"
#include "SandTable/Render/RenderAPI.h"
#include "PlatForm/OpenGL/Buffer/OpenGLIndexBuffer.h"

SAND_TABLE_NAMESPACE_BEGIN

Ref<Buffer> IndexBuffer::Create(const std::vector<int>& vecIndex)
{
	switch (RenderAPI::GetAPIType())
	{
	case RenderAPI::APIType::OpenGL:
		return CreateRef<OpenGLIndexBuffer>(vecIndex);
	default:
		SAND_TABLE_ASSERT(false, "Unknown RenderAPI");
		return nullptr;
	}
}

const std::vector<int> IndexBuffer::GetIndex() const
{
	return m_vecIndex;
}

IndexBuffer::IndexBuffer(const std::vector<int>& vecIndex) :
	m_vecIndex(vecIndex),
	m_uiRenderID(0)
{

}

SAND_TABLE_NAMESPACE_END