#include "pch.h"
#include "PlatForm/OpenGL/imgui_impl_opengl3.h"
#include "SandTable/Core/Application.h"
#include "ImGuiLayer.h"
#include "GLFW/glfw3.h"

namespace SandTable
{
	ImGuiLayer::ImGuiLayer():Layer("ImguiLayer"), m_bShowDemoWindow(true)
	{

	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // Setup back-end capabilities flags
        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)
        io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;    // We can create multi-viewports on the Platform side (optional)
#if GLFW_HAS_MOUSE_PASSTHROUGH || (GLFW_HAS_WINDOW_HOVERED && defined(_WIN32))
        io.BackendFlags |= ImGuiBackendFlags_HasMouseHoveredViewport; // We can set io.MouseHoveredViewport correctly (optional, not easy)
#endif
        io.BackendPlatformName = "imgui_impl_glfw";

        //// Setup Platform/Renderer bindings
        //auto glfwWindow = Application::GetApplication()->GetWindow();

        //// Setup Platform/Renderer bindings
        //ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
        ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
        ImGui_ImplOpenGL3_Shutdown();
        //ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
	}

	void ImGuiLayer::OnUpdate()
	{
        ImGuiIO& io = ImGui::GetIO();
        auto& spApplication = Application::GetApplication();
        SAND_TABLE_ASSERT(spApplication, "Application in imgui is null");
        io.DisplaySize = ImVec2(spApplication->GetWindowWidth(), spApplication->GetWindowHeight());

        float fTime = static_cast<float>(glfwGetTime());
        io.DeltaTime = m_fTime > 0.f ? (fTime - m_fTime) : (1.0 / 60.f);
        m_fTime = fTime;

        ImGui_ImplOpenGL3_NewFrame();
        //ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (m_bShowDemoWindow)
            ImGui::ShowDemoWindow(&m_bShowDemoWindow);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
	}
}