#include "pch.h"
#include "RenderCommand.h"
#include "PlatForm/OpenGL/OpenGLRenderAPI.h"

namespace SandTable
{
	std::shared_ptr<RenderAPI> RenderCommand::m_spRenderAPI = RenderAPI::CreateRenderAPI();

	void RenderCommand::SetClearColor(const glm::vec4& vec4Color)
	{
		m_spRenderAPI->SetClearColor(vec4Color);
	}

	void RenderCommand::Clear()
	{
		m_spRenderAPI->Clear();
	}

	void RenderCommand::DrawVertex(const std::shared_ptr<VertexArray>& spVertexArray)
	{
		spVertexArray->Bind();
		m_spRenderAPI->DrawVertex(spVertexArray);
	}
}

