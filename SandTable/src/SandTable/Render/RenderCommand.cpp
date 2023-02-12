#include "pch.h"
#include "RenderCommand.h"
#include "PlatForm/OpenGL/OpenGLRenderAPI.h"
#include "SandTable/Render/Buffer/IndexBuffer.h"

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

void RenderCommand::DrawTriangles(const Ref<VertexArray>& spVertexArray, unsigned int uiIndexCount)
{
	spVertexArray->Bind();
	if (uiIndexCount == 0)
	{
		uiIndexCount = spVertexArray->GetIndexBuffer()->GetIndex().size();
	}
	m_spRenderAPI->DrawTriangles(uiIndexCount);
}

void RenderCommand::DrawLines(const Ref<VertexArray>& spVertexArray, unsigned int uiIndexCount)
{
	spVertexArray->Bind();
	m_spRenderAPI->DrawLines(uiIndexCount);
}

void RenderCommand::SetLineWidth(float fWidth)
{
	m_spRenderAPI->SetLineWidth(fWidth);
}
SAND_TABLE_NAMESPACE_END
