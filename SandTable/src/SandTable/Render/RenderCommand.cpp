#include "pch.h"
#include "RenderCommand.h"
#include "PlatForm/OpenGL/OpenGLRenderAPI.h"

SAND_TABLE_NAMESPACE_BEGIN

Ref<RenderAPI> RenderCommand::m_spRenderAPI = RenderAPI::CreateRenderAPI();
void RenderCommand::Init()
{
	m_spRenderAPI->Init();
}
void RenderCommand::SetClearColor(const glm::vec4& vec4Color)
{
	m_spRenderAPI->SetClearColor(vec4Color);
}

void RenderCommand::SetViewPort(unsigned int uiWidth, unsigned int uiHeight)
{
	m_spRenderAPI->SetViewPort(uiWidth, uiHeight);
}

void RenderCommand::Clear()
{
	m_spRenderAPI->Clear();
}

void RenderCommand::DrawVertex(const Ref<VertexArray>& spVertexArray,unsigned int uiIndexCount)
{
	spVertexArray->Bind();
	m_spRenderAPI->DrawElements(uiIndexCount);
}

void RenderCommand::DrawVertex(const Ref<VertexArray>& spVertexArray)
{
	unsigned int uiIndexCount = spVertexArray->GetIndexBuffer()->GetIndex().size();
	DrawVertex(spVertexArray, uiIndexCount);
}

SAND_TABLE_NAMESPACE_END
