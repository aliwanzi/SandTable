#include "RayTracingEditorLayer.h"
#include <imgui/imgui.h>

RayTracingEditorLayer::RayTracingEditorLayer() :
	m_vec2RenderViewPortSize(ImVec2(0.0f, 0.0f)),
	m_spImage(nullptr),
	m_spTimer(CreateRef<Timer>()),
	m_fLastRenderTime(0.f)
{

}

void RayTracingEditorLayer::OnAttach()
{
	
}

void RayTracingEditorLayer::OnImGuiRender()
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
	ImGuiStyle& style = ImGui::GetStyle();
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 370.f;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	style.WindowMinSize.x = minWinSizeX;
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit")) 
			{ 
				Application::GetApplication()->Close(); 
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::Begin("Settints");
	ImGui::Text("Last render: %.3fms", m_fLastRenderTime);
	if (ImGui::Button("Render"))
	{
		Render();
	}
	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
	ImGui::Begin("ViewPort");
	
	m_vec2RenderViewPortSize.x = ImGui::GetContentRegionAvail().x;
	m_vec2RenderViewPortSize.y = ImGui::GetContentRegionAvail().y;

	if (m_spImage != nullptr)
	{
		ImGui::Image((void*)m_spImage->GetImage(), m_vec2RenderViewPortSize, ImVec2(0, 1), ImVec2(1, 0));
	}

	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::End();

	Render();
}

void RayTracingEditorLayer::Render()
{
	m_spTimer->Reset();
	if (m_spImage == nullptr || m_vec2RenderViewPortSize.x != m_spImage->GetWidth()
		|| m_vec2RenderViewPortSize.y != m_spImage->GetHeight())
	{
		m_spImage = CreateRef<Image>(m_vec2RenderViewPortSize.x, m_vec2RenderViewPortSize.y, InternalFormat::RGBA32F, DataFormat::RGBA);
	}

	auto pImageData = m_spImage->GetImageData();
	for (auto i = 0; i < m_vec2RenderViewPortSize.x * m_vec2RenderViewPortSize.y; i++)
	{
		pImageData[i] = Random::Uint32();
		pImageData[i] |= 0xff000000;
	}

	m_fLastRenderTime = m_spTimer->ElapsedMillis();
}