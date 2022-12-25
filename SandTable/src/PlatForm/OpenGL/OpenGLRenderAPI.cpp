#include "pch.h"
#include "OpenGLRenderAPI.h"
#include "GL/gl3w.h"
#include "SandTable/Debug/Instrumentor.h"

SAND_TABLE_NAMESPACE_BEGIN

void OpenGLRenderAPI::Init()
{
	SAND_TABLE_PROFILE_FUNCTION();

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLRenderAPI::SetClearColor(const glm::vec4& vec4Color)
{
	glClearColor(vec4Color.r, vec4Color.g, vec4Color.b, vec4Color.a);
}

void OpenGLRenderAPI::SetViewPort(unsigned int uiWidth, unsigned int uiHeight)
{
	glViewport(0, 0, uiWidth, uiHeight);
}

void OpenGLRenderAPI::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLRenderAPI::DrawVertex(const Ref<VertexArray>& spVertexArray, uint32_t uiIndexCount)
{
	auto uiCount = uiIndexCount == 0 ? spVertexArray->GetIndexBuffer()->GetIndex().size() : uiIndexCount;
	glDrawElements(GL_TRIANGLES, uiCount, GL_UNSIGNED_INT, nullptr);
}

SAND_TABLE_NAMESPACE_END
