#include "pch.h"
#include "Render.h"
#include "SandTable/Render/RenderCommand.h"

namespace SandTable
{
	std::shared_ptr<Render::SceneData> Render::m_spSceneData = std::make_shared<Render::SceneData>();

	void Render::BeginScene(const std::shared_ptr<Camera>& spCamera)
	{
		m_spSceneData->m_mat4ViewProjectionMatrix = spCamera->GetViewProjectionMatrix();
	}

	void Render::EndScene()
	{

	}

	void Render::Submit(const std::shared_ptr<Shader>& spShader, const std::shared_ptr<VertexArray>& spVertexArray, const glm::mat4& mat4Transform)
	{
		spShader->Bind();
		spShader->SetMat4("ViewProjection", m_spSceneData->m_mat4ViewProjectionMatrix);
		spShader->SetMat4("Transform", mat4Transform);
		RenderCommand::DrawVertex(spVertexArray);
	}
}


