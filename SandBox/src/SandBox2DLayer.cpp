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
	SAND_TABLE_PROFILE_SCOPE("SandBox2DLayer::OnUpdate");

	{
		SAND_TABLE_PROFILE_SCOPE("CameraController::OnUpdate");
		m_spOrthoGraphicCameraController->OnUpdate(timeStep);
	}

	{
		SAND_TABLE_PROFILE_SCOPE("Render Prep");
		RenderCommand::SetClearColor(glm::vec4(glm::vec3(0.1f), 1.0f));
		RenderCommand::Clear();
	}

	{
		static float fRotation = 0.f;
		fRotation += timeStep * 20.f;
		SAND_TABLE_PROFILE_SCOPE("Render Draw");
		Render2D::BeginScene(m_spOrthoGraphicCameraController->GetCamera());
		Render2D::DrawQuad(glm::vec2( 1.f, 0.f), -45.f, glm::vec2(0.8f, 0.8f), glm::vec4(0.8f, 0.2f, 0.3f, 1.0f));
		Render2D::DrawQuad(glm::vec2(-1.f, 0.f), 0.f, glm::vec2(0.8f, 0.8f), glm::vec4(0.8f, 0.2f, 0.3f, 1.0f));
		Render2D::DrawQuad(glm::vec2(0.5f, -0.5f), 0.f, glm::vec2(0.5f, 0.75f), glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));
		Render2D::DrawQuad(glm::vec3(0.f, 0.f, -0.1f), 0.f, glm::vec2(10.f, 10.f), m_spTexture, 10.f);
		Render2D::DrawQuad(glm::vec2(-2.f, 0.f), fRotation, glm::vec2(1.f, 1.f), m_spTexture, 20.f);
		Render2D::EndScene();
	}
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
