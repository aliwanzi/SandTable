#include "pch.h"
#include "Render.h"
#include "SandTable/Render/RenderCommand.h"

SAND_TABLE_NAMESPACE_BEGIN

Ref<Render::SceneData> Render::m_spSceneData = CreateRef<Render::SceneData>();
void Render::Init()
{
	RenderCommand::Init();
}

void Render::OnWindowResize(unsigned int uiWidth, unsigned int uiHeight)
{
	RenderCommand::SetViewPort(uiWidth, uiHeight);
}

void Render::BeginScene(const Ref<Camera>& spCamera)
{
	m_spSceneData->m_mat4ViewProjectionMatrix = spCamera->GetViewProjectionMatrix();
}

void Render::EndScene()
{

}

void Render::Submit(const Ref<Shader>& spShader, const Ref<VertexArray>& spVertexArray, const glm::mat4& mat4Transform)
{
	spShader->Bind();
	spShader->SetMat4("ViewProjection", m_spSceneData->m_mat4ViewProjectionMatrix);
	spShader->SetMat4("Transform", mat4Transform);
	RenderCommand::DrawTriangles(spVertexArray);
}

SAND_TABLE_NAMESPACE_END
