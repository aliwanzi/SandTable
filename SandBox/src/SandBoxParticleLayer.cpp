#include "SandBoxParticleLayer.h"

SandBoxParticleLayer::SandBoxParticleLayer() :m_vec4Color(glm::vec4(0.2f, 0.3f, 0.8f, 1.0f)), m_bRenderWindowActive(true)
{
	m_spOrthoGraphicCameraController = CreateRef<OrthoGraphicCameraController>
		(Application::GetApplication()->GetWindowWidth(), Application::GetApplication()->GetWindowHeight());
}

SandBoxParticleLayer::~SandBoxParticleLayer()
{
}

void SandBoxParticleLayer::OnAttach()
{
	m_spTexture = Texture2D::Create("assets/textures/Checkerboard.png");
	m_spTextureStar = Texture2D::Create("assets/textures/Star.png");

	m_spTextureSprite = Texture2D::Create("assets/games/textures/RPGpack_sheet_2X.png");
	m_spSubTexStairs = CreateRef<SubTexture2D>(m_spTextureSprite, glm::vec2(7, 6), glm::vec2(128, 128));
	m_spSubTexBarrel = CreateRef<SubTexture2D>(m_spTextureSprite, glm::vec2(1, 11), glm::vec2(128, 128));
	m_spSubTexTree = CreateRef<SubTexture2D>(m_spTextureSprite, glm::vec2(2, 1), glm::vec2(128, 128), glm::vec2(1.f, 2.f));

	m_spParticleSystem2D = CreateRef<ParticleSystem2D>();

	m_Particle.Position = glm::vec2(0.0f);
	m_Particle.Velocity = glm::vec2(0.0f);
	m_Particle.VelocityVariation = glm::vec2(3.0f, 1.0f);
	m_Particle.ColorBegin = glm::vec4(254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f);
	m_Particle.ColorEnd = glm::vec4(254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f);
	m_Particle.SizeBegin = 0.5f;
	m_Particle.SizeEnd = 0.0f;
	m_Particle.SizeVariation = 0.3f;
	m_Particle.LifeTime = 1.0f;
	m_Particle.ParticleType = ParticleRenderType::TEXTURE;
	m_Particle.RefTexture = m_spTextureStar;

	std::vector<FrameBufferTextureSpecification> vecFrameBufferTextureSpecification =
	{
		FramebufferTextureFormat::RGBA8,
		FramebufferTextureFormat::RED_INTERGER,
		FramebufferTextureFormat::DEPTH24_STENCIL8
	};
	auto spFrameBufferAttachmentSpecification = CreateRef<FrameBufferAttachmentSpecification>(vecFrameBufferTextureSpecification);
	auto spFrameBufferSpecification = CreateRef<FrameBufferSpecification>
		(Application::GetApplication()->GetWindowWidth(), Application::GetApplication()->GetWindowHeight(), 
			spFrameBufferAttachmentSpecification);

	m_spFrameBuffer = FrameBuffer::Create(spFrameBufferSpecification);
}

void SandBoxParticleLayer::OnDetach()
{

}

void SandBoxParticleLayer::OnUpdate(const TimeStep& timeStep)
{
	SAND_TABLE_PROFILE_SCOPE("SandBoxParticleLayer::OnUpdate");

	if (m_bRenderWindowActive)
	{
		SAND_TABLE_PROFILE_SCOPE("CameraController::OnUpdate");
		m_spOrthoGraphicCameraController->OnUpdate(timeStep);
	}

	m_spFrameBuffer->Bind();

	Render2D::ResetStats();
	{
		SAND_TABLE_PROFILE_SCOPE("Render Prep");
		RenderCommand::SetClearColor(glm::vec4(glm::vec3(0.1f), 1.0f));
		RenderCommand::Clear();
	}

	//{
	//	static float fRotation = 0.f;
	//	fRotation += timeStep * 20.f;
	//	SAND_TABLE_PROFILE_SCOPE("Render Draw");
	//	Render2D::BeginScene(m_spOrthoGraphicCameraController->GetCamera());
	//	Render2D::DrawQuad(glm::vec2( 1.f, 0.f), glm::radians(-45.f), glm::vec2(0.8f, 0.8f), glm::vec4(0.8f, 0.2f, 0.3f, 1.0f));
	//	Render2D::DrawQuad(glm::vec2(-1.f, 0.f), 0.f, glm::vec2(0.8f, 0.8f), glm::vec4(0.8f, 0.2f, 0.3f, 1.0f));
	//	Render2D::DrawQuad(glm::vec2(0.5f, -0.5f), 0.f, glm::vec2(0.5f, 0.75f), glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));
	//	Render2D::DrawQuad(glm::vec3(0.f, 0.f, -0.1f), 0.f, glm::vec2(20.f, 20.f), m_spTexture, 10.f);
	//	Render2D::DrawQuad(glm::vec2(-2.f, 0.f), glm::radians(fRotation), glm::vec2(1.f, 1.f), m_spTexture, 20.f);
	//	Render2D::EndScene();

	//	Render2D::BeginScene(m_spOrthoGraphicCameraController->GetCamera());
	//	for (float y = -5.f; y < 5.f; y += 0.5f)
	//	{
	//		for (float x = -5.f; x < 5.f; x += 0.5f)
	//		{
	//			glm::vec4 color = glm::vec4((x + 5.f) / 10.f, 0.4f, (y + 5.0f) / 10.0f,0.7f);
	//			Render2D::DrawQuad(glm::vec2(x, y), 0.f, glm::vec2(0.45f), color);
	//		}
	//	}
	//	Render2D::EndScene();
	//}

	if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
	{
		auto mousePos = Input::GetMousePos();
		float width = Application::GetApplication()->GetWindowWidth();
		float height = Application::GetApplication()->GetWindowHeight();
		auto cameraBound = m_spOrthoGraphicCameraController->GetOrthoGraphicCameraBounds();
		auto cameraPos = m_spOrthoGraphicCameraController->GetCamera()->GetPositon();
		auto x = (mousePos.x / width) * cameraBound.GetWidth() - cameraBound.GetWidth() * 0.5f;
		auto y = cameraBound.GetHeight() * 0.5f - (mousePos.y / height) * cameraBound.GetHeight();
		m_Particle.Position = glm::vec2(cameraPos.x + x, cameraPos.y + y);
		for (int i = 0; i < 5; i++)
		{
			m_spParticleSystem2D->Emit(m_Particle);
		}
	}

	Render2D::BeginScene(m_spOrthoGraphicCameraController->GetCamera());
	Render2D::DrawQuad(glm::vec3(0.f, 0.f, 0.5f), 0.f, glm::vec2(1.f), m_spSubTexStairs);
	Render2D::DrawQuad(glm::vec3(1.f, 0.f, 0.5f), 0.f, glm::vec2(1.f), m_spSubTexBarrel);
	Render2D::DrawQuad(glm::vec3(-1.f, 0.f, 0.5f), 0.f, glm::vec2(1.f, 2.f), m_spSubTexTree);
	Render2D::EndScene();

	m_spParticleSystem2D->OnUpdate(timeStep);
	m_spParticleSystem2D->OnRender(m_spOrthoGraphicCameraController->GetCamera());

	m_spFrameBuffer->UnBind();
}

void SandBoxParticleLayer::OnImGuiRender()
{
	static bool opt_dockSpaceOpen = true;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &opt_dockSpaceOpen, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit")) { Application::GetApplication()->Close(); }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::End();

	ImGui::Begin("Settings");
	auto stats = Render2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Draw Quads: %d", stats.QuadCount);
	ImGui::Text("Draw Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Draw Indices: %d", stats.GetTotalIndexCount());
	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("viewport");
	m_bRenderWindowActive = ImGui::IsWindowHovered();
	Application::GetApplication()->BlockEvents(!m_bRenderWindowActive);
	auto contentRegion = ImGui::GetContentRegionAvail();
	//LOG_DEV_INFO("viewport size: {0},{1}", contentRegion.x, contentRegion.y);
	auto spFrameBuffer = std::dynamic_pointer_cast<FrameBuffer>(m_spFrameBuffer);
	SAND_TABLE_ASSERT(spFrameBuffer, "FrameBuffer is null in Edit Layer");
	spFrameBuffer->Resize(contentRegion.x, contentRegion.y);
	m_spOrthoGraphicCameraController->OnResize(contentRegion.x, contentRegion.y);
	auto uiTextureID = spFrameBuffer->GetColorAttachment();
	ImGui::Image((void*)uiTextureID, contentRegion, ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
	ImGui::PopStyleVar();
}

void SandBoxParticleLayer::OnEvent(Event& e)
{
	m_spOrthoGraphicCameraController->OnEvent(e);
}
