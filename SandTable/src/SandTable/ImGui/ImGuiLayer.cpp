#include "pch.h"
#include "SandTable/Core/Application.h"
#include "ImGuiLayer.h"
#include "GLFW/glfw3.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"
#include "SandTable/Debug/Instrumentor.h"

SAND_TABLE_NAMESPACE_BEGIN

ImGuiLayer::ImGuiLayer() :Layer("ImguiLayer"), m_bBlockEvents(true)
{

}

ImGuiLayer::~ImGuiLayer()
{
	LOG_DEV_INFO("ImGuiLayer Finalize");
}

void ImGuiLayer::OnAttach()
{
	SAND_TABLE_PROFILE_FUNCTION();
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	//// Setup Platform/Renderer bindings
	const auto& upWindow = Application::GetApplication()->GetWindow();
	SAND_TABLE_ASSERT(upWindow, "ImGuiLayer get window failed");
	auto glfwWindow = static_cast<GLFWwindow*>(upWindow->GetNativeWindow());
	SAND_TABLE_ASSERT(glfwWindow, "ImGuiLayer get glfwWindow failed");

	//// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
	ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::OnDetach()
{
	SAND_TABLE_PROFILE_FUNCTION();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLayer::OnEvent(Event& e)
{
	e.Handle = m_bBlockEvents;
}

void ImGuiLayer::BeginNewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::EndNewFrame()
{
	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void ImGuiLayer::BlockEvents(bool bBlock)
{
	m_bBlockEvents = bBlock;
}

SAND_TABLE_NAMESPACE_END