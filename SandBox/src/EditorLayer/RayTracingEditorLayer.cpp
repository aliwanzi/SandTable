#include "RayTracingEditorLayer.h"
#include <imgui/imgui.h>

RayTracingEditorLayer::RayTracingEditorLayer() :
	m_vec2RenderViewPortSize(ImVec2(0.0f, 0.0f)),
	m_spTimer(CreateRef<Timer>()),
	m_fLastRenderTime(0.f),
	m_spObjectContainer(CreateRef<ObjectContainer>()),
	m_spRayTracingScene(CreateRef<RayTracingScene>()),
	m_spRayTracingCamera(CreateRef<RayTracingCamera>(45.f, 0.1f, 100.f, 0, 1))
{
	m_spRayTracingCamera->SetPosition(glm::dvec3(2.5, 1.5, 7));
	m_spRayTracingCamera->SetForwardDirection(glm::dvec3(-0.38, -0.1, -1.0));
	CreatTwoSphereScene();
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
	auto positon = m_spRayTracingCamera->GetPosition();
	if (ImGui::DragFloat3("Positon", glm::value_ptr(positon)))
	{
		m_spRayTracingCamera->SetPosition(positon);
	}
	auto direction = m_spRayTracingCamera->GetForwardDirection();
	if (ImGui::DragFloat3("Forward Direction", glm::value_ptr(direction)))
	{
		m_spRayTracingCamera->SetForwardDirection(direction);
	}
	ImGui::End();

	ImGui::Begin("SceneCase");

	if (ImGui::Button("TwoSphere"))
	{
		CreatTwoSphereScene();
	}

	if (ImGui::Button("FourSphere"))
	{
		CreatFourSphereScene();
	}

	if (ImGui::Button("MultiSpere"))
	{
		CreateMultiSphereScene();
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

void RayTracingEditorLayer::CreatTwoSphereScene()
{
	m_spRayTracingScene->ResetFrameIndex();

	auto spMaterial0 = CreateRef<Lambertian>(0);
	spMaterial0->SetAlbedo(glm::dvec3(0.8f, 0.8f, 0.f));
	auto spNoiseTexture = CreateRef<NoiseColorTexture>();
	spNoiseTexture->SetNoiseScale(4.0);
	spNoiseTexture->SetTurbDepth(7);
	spMaterial0->SetColorTexture(spNoiseTexture);
	m_spRayTracingScene->AddMaterial(spMaterial0);

	auto spSpherePrimitive0 = CreateRef<Sphere>(0);
	spSpherePrimitive0->SetPosition(glm::dvec3(0.f, -1000, 0));
	spSpherePrimitive0->SetRadius(1000.0f);
	spSpherePrimitive0->SetMaterialID(0);
	m_spObjectContainer->AddObject(spSpherePrimitive0);

	auto spSpherePrimitive1 = CreateRef<Sphere>(1);
	spSpherePrimitive1->SetPosition(glm::dvec3(0.f, 2.f, 0.f));
	spSpherePrimitive1->SetRadius(2);
	spSpherePrimitive1->SetMaterialID(0);
	m_spObjectContainer->AddObject(spSpherePrimitive1);

	m_spRayTracingScene->SetObjectContainer(m_spObjectContainer);
}


void RayTracingEditorLayer::CreatFourSphereScene()
{
	m_spRayTracingScene->ResetFrameIndex();

	auto spMaterial0 = CreateRef<Lambertian>(0);
	spMaterial0->SetAlbedo(glm::dvec3(0.8f, 0.8f, 0.f));
	m_spRayTracingScene->AddMaterial(spMaterial0);

	auto spMaterial1 = CreateRef<Lambertian>(1);
	spMaterial1->SetAlbedo(glm::dvec3(0.1, 0.2, 0.5));
	m_spRayTracingScene->AddMaterial(spMaterial1);

	auto spMaterial2 = CreateRef<Dielectric>(2);
	spMaterial2->SetMetallic(1.5);
	m_spRayTracingScene->AddMaterial(spMaterial2);

	auto spMaterial3 = CreateRef<Metal>(3);
	spMaterial3->SetAlbedo(glm::dvec3(0.8f, 0.6f, 0.2f));
	spMaterial3->SetRoughness(0.f);
	m_spRayTracingScene->AddMaterial(spMaterial3);

	auto spSpherePrimitive0 = CreateRef<Sphere>(0);
	spSpherePrimitive0->SetPosition(glm::dvec3(0.f, -100.5f, -1.f));
	spSpherePrimitive0->SetRadius(100.0f);
	spSpherePrimitive0->SetMaterialID(0);
	m_spObjectContainer->AddObject(spSpherePrimitive0);

	auto spSpherePrimitive1 = CreateRef<Sphere>(1);
	spSpherePrimitive1->SetPosition(glm::dvec3(0.f, 0.f, -1.f));
	spSpherePrimitive1->SetRadius(0.5f);
	spSpherePrimitive1->SetMaterialID(1);
	m_spObjectContainer->AddObject(spSpherePrimitive1);

	auto spSpherePrimitive2 = CreateRef<Sphere>(2);
	spSpherePrimitive2->SetPosition(glm::dvec3(-1.f, 0.f, -1.f));
	spSpherePrimitive2->SetRadius(0.5f);
	spSpherePrimitive2->SetMaterialID(2);
	m_spObjectContainer->AddObject(spSpherePrimitive2);

	auto spSpherePrimitive3 = CreateRef<Sphere>(3);
	spSpherePrimitive3->SetPosition(glm::dvec3(-1.f, 0.f, -1.f));
	spSpherePrimitive3->SetRadius(-0.4f);
	spSpherePrimitive3->SetMaterialID(2);
	m_spObjectContainer->AddObject(spSpherePrimitive3);

	auto spSpherePrimitive4 = CreateRef<Sphere>(4);
	spSpherePrimitive4->SetPosition(glm::dvec3(1.f, 0.f, -1.f));
	spSpherePrimitive4->SetRadius(0.5f);
	spSpherePrimitive4->SetMaterialID(3);
	m_spObjectContainer->AddObject(spSpherePrimitive4);

	m_spRayTracingScene->SetObjectContainer(m_spObjectContainer);
}

void RayTracingEditorLayer::CreateMultiSphereScene()
{
	m_spRayTracingScene->ResetFrameIndex();

	int iMaterialNum(0);
	int iObjectNum(0);
	for (int a = -5; a < 5; a++)
	{
		for (int b = -5; b < 5; b++)
		{
			float fChooseMat = Random::Float();
			glm::dvec3 center(a + 0.9 * Random::Float(), 0.2, b + 0.9 * Random::Float());

			if (glm::length(center - glm::dvec3(4, 0.2, 0)) > 0.9)
			{

				if (fChooseMat < 0.8)
				{
					//diffuse
					auto moveCenter = center + glm::dvec3(0.f, 0.5f * Random::Float(), 0.f);
					auto spSphere = CreateRef<MovingSphere>(iObjectNum++);
					spSphere->SetPosition(center);
					spSphere->SetMovePosition(moveCenter);
					spSphere->SetStepBegin(0.f);
					spSphere->SetStepEnd(1.f);
					spSphere->SetRadius(0.2);
					spSphere->SetMaterialID(iMaterialNum);
					m_spObjectContainer->AddObject(spSphere);

					auto spLambertian = CreateRef<Lambertian>(iMaterialNum++);
					spLambertian->SetAlbedo(Random::Vec3());
					m_spRayTracingScene->AddMaterial(spLambertian);
				}
				else if (fChooseMat < 0.95)
				{
					auto spSphere = CreateRef<Sphere>(iObjectNum++);
					spSphere->SetPosition(center);
					spSphere->SetRadius(0.2);
					spSphere->SetMaterialID(iMaterialNum);
					m_spObjectContainer->AddObject(spSphere);

					//metal
					auto spLambertian = CreateRef<Metal>(iMaterialNum++);
					spLambertian->SetAlbedo(0.5 * Random::Vec3());
					spLambertian->SetRoughness(Random::Float() * 0.5);
					m_spRayTracingScene->AddMaterial(spLambertian);
				}
				else
				{
					auto spSphere = CreateRef<Sphere>(iObjectNum++);
					spSphere->SetPosition(center);
					spSphere->SetRadius(0.2);
					spSphere->SetMaterialID(iMaterialNum);
					m_spObjectContainer->AddObject(spSphere);

					//glass
					auto spDielectric = CreateRef<Dielectric>(iMaterialNum++);
					spDielectric->SetMetallic(1.5);
					m_spRayTracingScene->AddMaterial(spDielectric);
				}
			}
		}
	}

	auto spGroundSphere = CreateRef<Sphere>(iObjectNum++);
	spGroundSphere->SetPosition(glm::dvec3(0, -1000, 0));
	spGroundSphere->SetRadius(1000);
	spGroundSphere->SetMaterialID(iMaterialNum);
	m_spObjectContainer->AddObject(spGroundSphere);
	auto spGroundMaterial = CreateRef<Lambertian>(iMaterialNum++);
	spGroundMaterial->SetAlbedo(glm::dvec3(0.5f));
	auto spEven = CreateRef<SolidColorTexture>(glm::dvec3(0.2, 0.3, 0.1));
	auto spOdd = CreateRef<SolidColorTexture>(glm::dvec3(0.9));
	auto spCheckerColor = CreateRef<CheckerColorTexture>(spEven, spOdd);
	spGroundMaterial->SetColorTexture(spCheckerColor);
	m_spRayTracingScene->AddMaterial(spGroundMaterial);

	auto spDielectricSphere = CreateRef<Sphere>(iObjectNum++);
	spDielectricSphere->SetPosition(glm::dvec3(-4, 1, 0));
	spDielectricSphere->SetRadius(1);
	spDielectricSphere->SetMaterialID(iMaterialNum);
	m_spObjectContainer->AddObject(spDielectricSphere);
	auto spDielectric = CreateRef<Dielectric>(iMaterialNum++);
	spDielectric->SetMetallic(1.5);
	m_spRayTracingScene->AddMaterial(spDielectric);

	auto spLambertianSphere = CreateRef<Sphere>(iObjectNum++);
	spLambertianSphere->SetPosition(glm::dvec3(0, 1, 0));
	spLambertianSphere->SetRadius(1);
	spLambertianSphere->SetMaterialID(iMaterialNum);
	m_spObjectContainer->AddObject(spLambertianSphere);
	auto spLambertian = CreateRef<Lambertian>(iMaterialNum++);
	spLambertian->SetAlbedo(glm::dvec3(0.4, 0.2, 0.1));
	m_spRayTracingScene->AddMaterial(spLambertian);

	auto spMetalSphere = CreateRef<Sphere>(iObjectNum++);
	spMetalSphere->SetPosition(glm::dvec3(4, 1, 0));
	spMetalSphere->SetRadius(1);
	spMetalSphere->SetMaterialID(iMaterialNum);
	m_spObjectContainer->AddObject(spMetalSphere);
	auto spMetal = CreateRef<Metal>(iMaterialNum++);
	spMetal->SetAlbedo(glm::dvec3(0.7, 0.6, 0.5));
	m_spRayTracingScene->AddMaterial(spMetal);


	m_spRayTracingScene->SetObjectContainer(m_spObjectContainer);
}



