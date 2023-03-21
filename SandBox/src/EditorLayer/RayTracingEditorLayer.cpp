#include "RayTracingEditorLayer.h"
#include <imgui/imgui.h>

RayTracingEditorLayer::RayTracingEditorLayer() :
	m_vec2RenderViewPortSize(ImVec2(0.0f, 0.0f)),
	m_spTimer(CreateRef<Timer>()),
	m_fLastRenderTime(0.f),
	m_spRayTracingScene(std::make_shared<RayTracingScene>()),
	m_spRayTracingCamera(CreateRef<RayTracingCamera>(45.f, 0.1f, 100.f))
{
	auto spMaterial0 = CreateRef<Material>(MaterialType::Lambertian, 0);
	spMaterial0->SetAlbedo(glm::vec3(0.8f, 0.8f, 0.f));
	m_spRayTracingScene->AddMaterial(spMaterial0);

	auto spMaterial1 = CreateRef<Material>(MaterialType::Lambertian, 1);
	spMaterial1->SetAlbedo(glm::vec3(0.7f, 0.3f, 0.3f));
	m_spRayTracingScene->AddMaterial(spMaterial1);

	auto spMaterial2 = CreateRef<Material>(MaterialType::Metal, 2);
	spMaterial2->SetAlbedo(glm::vec3(0.8f));
	spMaterial2->SetRoughness(0.3f);
	m_spRayTracingScene->AddMaterial(spMaterial2);

	auto spMaterial3 = CreateRef<Material>(MaterialType::Metal, 3);
	spMaterial3->SetAlbedo(glm::vec3(0.8f, 0.6f, 0.2f));
	spMaterial3->SetRoughness(1.f);
	m_spRayTracingScene->AddMaterial(spMaterial3);

	auto spSpherePrimitive0 = CreateRef<SpherePrimitive>(glm::vec3(0.f, -100.5f, -1.f), 100.0f, 0);
	spSpherePrimitive0->SetMaterialID(0);
	m_spRayTracingScene->AddSpherePrimive(spSpherePrimitive0);

	auto spSpherePrimitive1 = CreateRef<SpherePrimitive>(glm::vec3(0.f, 0.f, -1.f), 0.5f, 1);
	spSpherePrimitive1->SetMaterialID(1);
	m_spRayTracingScene->AddSpherePrimive(spSpherePrimitive1);

	auto spSpherePrimitive2 = CreateRef<SpherePrimitive>(glm::vec3(-1.f, 0.f, -1.f), 0.5f, 2);
	spSpherePrimitive2->SetMaterialID(2);
	m_spRayTracingScene->AddSpherePrimive(spSpherePrimitive2);

	auto spSpherePrimitive3 = CreateRef<SpherePrimitive>(glm::vec3(1.f, 0.f, -1.f), 0.5f, 3);
	spSpherePrimitive3->SetMaterialID(3);
	m_spRayTracingScene->AddSpherePrimive(spSpherePrimitive3);
}

void RayTracingEditorLayer::OnAttach()
{

}

void RayTracingEditorLayer::OnUpdate(const TimeStep& ts)
{
	m_spTimer->Reset();
	if (m_vec2RenderViewPortSize.x > 0 && m_vec2RenderViewPortSize.y > 0)
	{
		m_spRayTracingCamera->OnResize(m_vec2RenderViewPortSize.x, m_vec2RenderViewPortSize.y);
		m_spRayTracingScene->OnViewPortResize(m_vec2RenderViewPortSize.x, m_vec2RenderViewPortSize.y);
		m_spRayTracingCamera->OnUpdate(ts);
		m_spRayTracingScene->OnUpdate(ts, m_spRayTracingCamera);
	}

	m_fLastRenderTime = m_spTimer->ElapsedMillis();
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

	ImGui::Begin("Settings");
	ImGui::Text("Last render: %.3fms", m_fLastRenderTime);
	ImGui::Checkbox("Accumulate", &m_spRayTracingScene->GetAccumulate());
	if (ImGui::Button("Reset"))
	{
		m_spRayTracingScene->ResetFrameIndex();
	}
	ImGui::Text("Frame Index: %d", m_spRayTracingScene->GetFrameIndex());
	ImGui::End();

	ImGui::Begin("Scene");
	auto& mapSphere = m_spRayTracingScene->GetSpherePrimives();
	auto& mapMaterial = m_spRayTracingScene->GetMaterials();

	glm::vec3 position(0.f);
	float radius(0.f);

	glm::vec3 albedo(0.f);
	float roughness(0.f);
	float metallic(0.f);
	for (auto& sphere : mapSphere)
	{
		position = sphere.second->GetPosition();
		radius = sphere.second->GetRadius();
		ImGui::PushID(sphere.first);
		if (ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f))
		{
			sphere.second->SetPosition(position);
		}
		if (ImGui::DragFloat("Radius", &radius, 0.1f))
		{
			sphere.second->SetRadius(radius);
		}

		auto material = mapMaterial.find(sphere.second->GetMaterialID())->second;
		albedo = material->GetAlbedo();
		roughness = material->GetRoughness();
		metallic = material->GetMetallic();
		if (ImGui::ColorEdit3("Albedo", glm::value_ptr(albedo)))
		{
			material->SetAlbedo(albedo);
		}
		if (ImGui::DragFloat("Roughness", &roughness, 0.01f, 0.0f, 1.f))
		{
			material->SetRoughness(roughness);
		}
		if (ImGui::DragFloat("Metallic", &metallic, 0.01f, 0.0f, 1.f))
		{
			material->SetMetallic(metallic);
		}

		ImGui::Separator();
		ImGui::PopID();
	}

	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
	ImGui::Begin("ViewPort");

	m_vec2RenderViewPortSize = ImGui::GetContentRegionAvail();

	auto image = m_spRayTracingScene->GetRenderImage();
	if (image > 0)
	{
		ImGui::Image((void*)image, m_vec2RenderViewPortSize, ImVec2(0, 1), ImVec2(1, 0));
	}

	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::End();
}