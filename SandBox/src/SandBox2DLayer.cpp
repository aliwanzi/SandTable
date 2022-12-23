#include "SandBox2DLayer.h"
#include <glm/gtc/type_ptr.hpp>

SandBox2DLayer::SandBox2DLayer():m_vec4Color(glm::vec4(0.2f, 0.3f, 0.8f, 1.0f))
{
	m_spOrthoGraphicCameraController = CreateRef<OrthoGraphicCameraController>
		(static_cast<float>(Application::GetApplication()->GetWindowWidth()) /
			static_cast<float>(Application::GetApplication()->GetWindowHeight()), true);

	m_spTexture = Texture2D::Create("assets/textures/Checkerboard.png");
}

SandBox2DLayer::~SandBox2DLayer()
{
}

void SandBox2DLayer::OnAttach()
{

}

void SandBox2DLayer::OnDetach()
{

}

void SandBox2DLayer::OnUpdate(const TimeStep& timeStep)
{
	m_spOrthoGraphicCameraController->OnUpdate(timeStep);

	RenderCommand::SetClearColor(glm::vec4(glm::vec3(0.1f), 1.0f));
	RenderCommand::Clear();

	Render2D::BeginScene(m_spOrthoGraphicCameraController->GetCamera());
	Render2D::DrawQuad(glm::vec2(-1.f), 0.f, glm::vec2(0.8f, 0.8f), glm::vec4(0.8f, 0.2f, 0.3f, 1.0f));
	Render2D::DrawQuad(glm::vec2(0.5f, -0.5f), 0.f, glm::vec2(0.5f, 0.7f), glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));
	Render2D::DrawQuad(glm::vec3(0.0f, 0.0f, -0.1f), 0.f, glm::vec2(10.f, 10.f), m_spTexture);
	Render::EndScene();
}

void SandBox2DLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_vec4Color));
	ImGui::End();
}

void SandBox2DLayer::OnEvent(Event& e)
{
	m_spOrthoGraphicCameraController->OnEvent(e);
}
