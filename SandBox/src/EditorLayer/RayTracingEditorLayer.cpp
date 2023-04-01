#include "RayTracingEditorLayer.h"
#include <imgui/imgui.h>

RayTracingEditorLayer::RayTracingEditorLayer() :
	m_vec2RenderViewPortSize(ImVec2(0.0f, 0.0f)),
	m_spTimer(CreateRef<Timer>()),
	m_fLastRenderTime(0.f),
	m_spObjectContainer(CreateRef<ObjectContainer>()),
	m_spRayTracingScene(CreateRef<RayTracingScene>()),
	m_spRayTracingCamera(CreateRef<RayTracingCamera>(40.f, 0.1f, 100.f, 0, 1))
{
	m_spRayTracingCamera->SetPosition(glm::dvec3(2.5, 1.5, 7));
	m_spRayTracingCamera->SetForwardDirection(glm::dvec3(-0.38, -0.1, -1.0));
	CreateFinalScene();
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

	if (ImGui::Button("Save Image"))
	{
		m_spRayTracingScene->SaveImage();
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
	m_spRayTracingScene->SetBackGroundColor(glm::dvec3(0.7, 0.8, 1.0));

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

void RayTracingEditorLayer::CreatEarthSphereScene()
{
	m_spRayTracingScene->ResetFrameIndex();
	m_spRayTracingScene->SetBackGroundColor(glm::dvec3(0.7, 0.8, 1.0));

	auto spMaterial0 = CreateRef<Lambertian>(0);
	spMaterial0->SetAlbedo(glm::dvec3(0.8f, 0.8f, 0.f));
	auto spImageColorTexture = CreateRef<ImageColorTexture>("resources/texture/earthmap.jpg");
	spMaterial0->SetColorTexture(spImageColorTexture);
	m_spRayTracingScene->AddMaterial(spMaterial0);

	auto spSpherePrimitive0 = CreateRef<Sphere>(0);
	spSpherePrimitive0->SetPosition(glm::dvec3(0.f, 0, 0));
	spSpherePrimitive0->SetRadius(2.0f);
	spSpherePrimitive0->SetMaterialID(0);
	m_spObjectContainer->AddObject(spSpherePrimitive0);

	m_spRayTracingScene->SetObjectContainer(m_spObjectContainer);
}


void RayTracingEditorLayer::CreatFourSphereScene()
{
	m_spRayTracingScene->ResetFrameIndex();
	m_spRayTracingScene->SetBackGroundColor(glm::dvec3(0.7, 0.8, 1.0));

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
	spMaterial3->SetRoughness(0.0f);
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
	m_spRayTracingScene->SetBackGroundColor(glm::dvec3(0.7, 0.8, 1.0));
	m_spRayTracingCamera->SetPosition(glm::dvec3(4.698,0.643,-5.733));
	m_spRayTracingCamera->SetForwardDirection(glm::dvec3(-0.521, -0.172, 0.923));

	int iMaterialNum(0);
	int iObjectNum(0);
	for (int a = -10; a < 10; a++)
	{
		for (int b = -10; b < 10; b++)
		{
			float fChooseMat = Random::Float();
			glm::dvec3 center(a + 0.9 * Random::Float(), 0.2, b + 0.9 * Random::Float());

			if (glm::length(center - glm::dvec3(4, 0.2, 0)) > 0.9)
			{

				if (fChooseMat < 0.6)
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
	spDielectricSphere->SetPosition(glm::dvec3(0, 1, 0));
	spDielectricSphere->SetRadius(1);
	spDielectricSphere->SetMaterialID(iMaterialNum);
	m_spObjectContainer->AddObject(spDielectricSphere);

	auto spSpherePrimitive3 = CreateRef<Sphere>(iObjectNum++);
	spSpherePrimitive3->SetPosition(glm::dvec3(0, 1, 0));
	spSpherePrimitive3->SetRadius(-0.5f);
	spSpherePrimitive3->SetMaterialID(iMaterialNum);
	m_spObjectContainer->AddObject(spSpherePrimitive3);

	auto spDielectric = CreateRef<Dielectric>(iMaterialNum++);
	spDielectric->SetMetallic(1.5);
	m_spRayTracingScene->AddMaterial(spDielectric);

	auto spLambertianSphere = CreateRef<Sphere>(iObjectNum++);
	spLambertianSphere->SetPosition(glm::dvec3(-4, 1, 0));
	spLambertianSphere->SetRadius(1);
	spLambertianSphere->SetMaterialID(iMaterialNum);
	m_spObjectContainer->AddObject(spLambertianSphere);
	auto spLambertian = CreateRef<Lambertian>(iMaterialNum++);
	auto spNoiseTexture0 = CreateRef<NoiseColorTexture>();
	spNoiseTexture0->SetNoiseScale(4.0);
	spNoiseTexture0->SetTurbDepth(7);
	spLambertian->SetColorTexture(spNoiseTexture0);
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

void RayTracingEditorLayer::CreateSampleLightScene()
{
	m_spRayTracingScene->ResetFrameIndex();
	m_spRayTracingScene->SetBackGroundColor(glm::dvec3(0.7, 0.8, 1.0));

	auto spMaterial0 = CreateRef<Lambertian>(0);
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

	auto spColorTexture = CreateRef<SolidColorTexture>(glm::dvec3(4));
	auto spDiffuseLight = CreateRef<DiffuseLight>(spColorTexture, 1);
	m_spRayTracingScene->AddMaterial(spDiffuseLight);

	auto spXYRectangle = CreateRef<XYRectangle>(2);
	spXYRectangle->SetXYPoints(glm::dvec2(3, 1), glm::dvec2(5, 3));
	spXYRectangle->SetZPos(-2);
	spXYRectangle->SetMaterialID(1);
	m_spObjectContainer->AddObject(spXYRectangle);

	auto spSpherePrimitive2 = CreateRef<Sphere>(3);
	spSpherePrimitive2->SetPosition(glm::dvec3(0.f, 7.f, 0.f));
	spSpherePrimitive2->SetRadius(2);
	spSpherePrimitive2->SetMaterialID(1);
	m_spObjectContainer->AddObject(spSpherePrimitive2);

	m_spRayTracingScene->SetObjectContainer(m_spObjectContainer);
}

void RayTracingEditorLayer::CreateCornellBoxScene()
{
	m_spRayTracingCamera->SetPosition(glm::vec3(278, 278, -600));
	m_spRayTracingCamera->SetForwardDirection(glm::vec3(278, 278, 0) - glm::vec3(278, 278, -800));

	auto spRed = CreateRef<Lambertian>(0);
	spRed->SetAlbedo(glm::dvec3(0.65,0.05,0.05));
	m_spRayTracingScene->AddMaterial(spRed);

	auto spWhite = CreateRef<Lambertian>(1);
	spWhite->SetAlbedo(glm::dvec3(0.73));
	m_spRayTracingScene->AddMaterial(spWhite);

	auto spGreen = CreateRef<Lambertian>(2);
	spGreen->SetAlbedo(glm::dvec3(0.12, 0.45, 0.15));
	m_spRayTracingScene->AddMaterial(spGreen);

	auto spColorTexture = CreateRef<SolidColorTexture>(glm::dvec3(15));
	auto spDiffuseLight = CreateRef<DiffuseLight>(spColorTexture, 3);
	m_spRayTracingScene->AddMaterial(spDiffuseLight);

	auto spYZRectangle0 = CreateRef<YZRectangle>(0);
	spYZRectangle0->SetYZPoints(glm::dvec2(0), glm::dvec2(555));
	spYZRectangle0->SetXPos(555);
	spYZRectangle0->SetMaterialID(2);
	m_spObjectContainer->AddObject(spYZRectangle0);

	auto spYZRectangle1 = CreateRef<YZRectangle>(1);
	spYZRectangle1->SetYZPoints(glm::dvec2(0), glm::dvec2(555));
	spYZRectangle1->SetXPos(0);
	spYZRectangle1->SetMaterialID(0);
	m_spObjectContainer->AddObject(spYZRectangle1);

	auto spXZRectangle0 = CreateRef<XZRectangle>(2);
	spXZRectangle0->SetXZPoints(glm::dvec2(213, 227), glm::dvec2(343, 332));
	spXZRectangle0->SetYPos(554);
	spXZRectangle0->SetMaterialID(3);
	m_spObjectContainer->AddObject(spXZRectangle0);

	auto spXZRectangle1 = CreateRef<XZRectangle>(3);
	spXZRectangle1->SetXZPoints(glm::dvec2(0), glm::dvec2(555));
	spXZRectangle1->SetYPos(0);
	spXZRectangle1->SetMaterialID(1);
	m_spObjectContainer->AddObject(spXZRectangle1);

	auto spXZRectangle2 = CreateRef<XZRectangle>(4);
	spXZRectangle2->SetXZPoints(glm::dvec2(0), glm::dvec2(555));
	spXZRectangle2->SetYPos(555);
	spXZRectangle2->SetMaterialID(1);
	m_spObjectContainer->AddObject(spXZRectangle2);

	auto spXYRectangle = CreateRef<XYRectangle>(5);
	spXYRectangle->SetXYPoints(glm::dvec2(0), glm::dvec2(555));
	spXYRectangle->SetZPos(555);
	spXYRectangle->SetMaterialID(1);
	m_spObjectContainer->AddObject(spXYRectangle);

	auto spBox0 = CreateRef<Box>(glm::dvec3(0), glm::dvec3(165, 330, 165), 6);
	spBox0->SetMaterialID(1);
	spBox0->SetTranslate(glm::dvec3(265, 0, 295));
	spBox0->SetRotateY(15);
	m_spObjectContainer->AddObject(spBox0);

	auto spBox1 = CreateRef<Box>(glm::dvec3(0), glm::dvec3(165), 7);
	spBox1->SetMaterialID(1);
	spBox1->SetTranslate(glm::dvec3(130, 0, 130));
	spBox1->SetRotateY(-18);
	m_spObjectContainer->AddObject(spBox1);

	m_spRayTracingScene->SetObjectContainer(m_spObjectContainer);
}

void RayTracingEditorLayer::CreateCornellBoxSmokeScene()
{
	m_spRayTracingCamera->SetPosition(glm::vec3(278, 278, -600));
	m_spRayTracingCamera->SetForwardDirection(glm::vec3(278, 278, 0) - glm::vec3(278, 278, -800));

	auto spRed = CreateRef<Lambertian>(0);
	spRed->SetAlbedo(glm::dvec3(0.65, 0.05, 0.05));
	m_spRayTracingScene->AddMaterial(spRed);

	auto spWhite = CreateRef<Lambertian>(1);
	spWhite->SetAlbedo(glm::dvec3(0.73));
	m_spRayTracingScene->AddMaterial(spWhite);

	auto spGreen = CreateRef<Lambertian>(2);
	spGreen->SetAlbedo(glm::dvec3(0.12, 0.45, 0.15));
	m_spRayTracingScene->AddMaterial(spGreen);

	auto spColorTexture0 = CreateRef<SolidColorTexture>(glm::dvec3(7));
	auto spDiffuseLight = CreateRef<DiffuseLight>(spColorTexture0, 3);
	m_spRayTracingScene->AddMaterial(spDiffuseLight);

	auto spColorTexture1 = CreateRef<SolidColorTexture>(glm::dvec3(0));
	auto spIsotropic0 = CreateRef<Isotropic>(spColorTexture1, 4);
	m_spRayTracingScene->AddMaterial(spIsotropic0);

	auto spColorTexture2 = CreateRef<SolidColorTexture>(glm::dvec3(1));
	auto spIsotropic1 = CreateRef<Isotropic>(spColorTexture2, 5);
	m_spRayTracingScene->AddMaterial(spIsotropic1);

	auto spYZRectangle0 = CreateRef<YZRectangle>(0);
	spYZRectangle0->SetYZPoints(glm::dvec2(0), glm::dvec2(555));
	spYZRectangle0->SetXPos(555);
	spYZRectangle0->SetMaterialID(2);
	m_spObjectContainer->AddObject(spYZRectangle0);

	auto spYZRectangle1 = CreateRef<YZRectangle>(1);
	spYZRectangle1->SetYZPoints(glm::dvec2(0), glm::dvec2(555));
	spYZRectangle1->SetXPos(0);
	spYZRectangle1->SetMaterialID(0);
	m_spObjectContainer->AddObject(spYZRectangle1);

	auto spXZRectangle0 = CreateRef<XZRectangle>(2);
	spXZRectangle0->SetXZPoints(glm::dvec2(113, 127), glm::dvec2(443, 432));
	spXZRectangle0->SetYPos(554);
	spXZRectangle0->SetMaterialID(3);
	m_spObjectContainer->AddObject(spXZRectangle0);

	auto spXZRectangle1 = CreateRef<XZRectangle>(3);
	spXZRectangle1->SetXZPoints(glm::dvec2(0), glm::dvec2(555));
	spXZRectangle1->SetYPos(0);
	spXZRectangle1->SetMaterialID(1);
	m_spObjectContainer->AddObject(spXZRectangle1);

	auto spXZRectangle2 = CreateRef<XZRectangle>(4);
	spXZRectangle2->SetXZPoints(glm::dvec2(0), glm::dvec2(555));
	spXZRectangle2->SetYPos(555);
	spXZRectangle2->SetMaterialID(1);
	m_spObjectContainer->AddObject(spXZRectangle2);

	auto spXYRectangle = CreateRef<XYRectangle>(5);
	spXYRectangle->SetXYPoints(glm::dvec2(0), glm::dvec2(555));
	spXYRectangle->SetZPos(555);
	spXYRectangle->SetMaterialID(1);
	m_spObjectContainer->AddObject(spXYRectangle);

	auto spBox0 = CreateRef<Box>(glm::dvec3(0), glm::dvec3(165, 330, 165), 6);
	spBox0->SetMaterialID(4);
	spBox0->SetTranslate(glm::dvec3(265, 0, 295));
	spBox0->SetRotateY(15);
	auto spConstantMedium0 = CreateRef<ConstantMedium>(spBox0, 0.01);
	m_spObjectContainer->AddObject(spConstantMedium0);

	auto spBox1 = CreateRef<Box>(glm::dvec3(0), glm::dvec3(165), 7);
	spBox1->SetMaterialID(5);
	spBox1->SetTranslate(glm::dvec3(130, 0, 130));
	spBox1->SetRotateY(-18);
	auto spConstantMedium1 = CreateRef<ConstantMedium>(spBox1, 0.01);
	m_spObjectContainer->AddObject(spConstantMedium1);

	m_spRayTracingScene->SetObjectContainer(m_spObjectContainer);
}

void RayTracingEditorLayer::CreateFinalScene()
{
	m_spRayTracingCamera->SetPosition(glm::dvec3(478, 278, -600));
	m_spRayTracingCamera->SetForwardDirection(glm::dvec3(278, 278, 0) - glm::dvec3(478, 278, -600));

	//m_spRayTracingScene->SetBackGroundColor(glm::dvec3(0.7, 0.8, 1.0));

	auto spColorTexture0 = CreateRef<SolidColorTexture>(glm::dvec3(7));
	auto spDiffuseLight = CreateRef<DiffuseLight>(spColorTexture0, 0);
	m_spRayTracingScene->AddMaterial(spDiffuseLight);

	auto spXZRectangle2 = CreateRef<XZRectangle>(0);
	spXZRectangle2->SetXZPoints(glm::dvec2(123,147), glm::dvec2(423,412));
	spXZRectangle2->SetYPos(554);
	spXZRectangle2->SetMaterialID(0);
	m_spObjectContainer->AddObject(spXZRectangle2);

	auto spLambertian = CreateRef<Lambertian>(1);
	spLambertian->SetAlbedo(glm::dvec3(0.7, 0.3, 0.1));
	m_spRayTracingScene->AddMaterial(spLambertian);

	auto center1 = glm::dvec3(400, 400, 200);
	auto center2 = center1 + glm::dvec3(30, 0, 0);
	auto spSphere = CreateRef<MovingSphere>(1);
	spSphere->SetPosition(center1);
	spSphere->SetMovePosition(center2);
	spSphere->SetStepBegin(0.f);
	spSphere->SetStepEnd(1.f);
	spSphere->SetRadius(50);
	spSphere->SetMaterialID(1);
	m_spObjectContainer->AddObject(spSphere);

	auto spDielectric = CreateRef<Dielectric>(2);
	spDielectric->SetMetallic(1.5);
	m_spRayTracingScene->AddMaterial(spDielectric);

	auto spSphere0 = CreateRef<Sphere>(2);
	spSphere0->SetPosition(glm::dvec3(260, 150, 45));
	spSphere0->SetRadius(50);
	spSphere0->SetMaterialID(2);
	m_spObjectContainer->AddObject(spSphere0);

	auto spMetal = CreateRef<Metal>(3);
	spMetal->SetAlbedo(glm::dvec3(0.8, 0.8, 0.9));
	spMetal->SetRoughness(1.0);
	m_spRayTracingScene->AddMaterial(spMetal);

	auto spSphere1 = CreateRef<Sphere>(3);
	spSphere1->SetPosition(glm::dvec3(0, 150, 145));
	spSphere1->SetRadius(50);
	spSphere1->SetMaterialID(3);
	m_spObjectContainer->AddObject(spSphere1);

	auto spSphere2 = CreateRef<Sphere>(3);
	spSphere2->SetPosition(glm::dvec3(360, 150, 145));
	spSphere2->SetRadius(70);
	spSphere2->SetMaterialID(2);
	m_spObjectContainer->AddObject(spSphere2);

	auto spColorTexture1 = CreateRef<SolidColorTexture>(glm::dvec3(0.2, 0.4, 0.9));
	auto spIsotropic0 = CreateRef<Isotropic>(spColorTexture1, 4);
	m_spRayTracingScene->AddMaterial(spIsotropic0);

	auto spSphere3 = CreateRef<Sphere>(4);
	spSphere3->SetPosition(glm::dvec3(360, 150, 145));
	spSphere3->SetRadius(70);
	spSphere3->SetMaterialID(4);
	auto spConstantMedium0 = CreateRef<ConstantMedium>(spSphere3, 0.2);
	m_spObjectContainer->AddObject(spConstantMedium0);

	auto spColorTexture2 = CreateRef<SolidColorTexture>(glm::dvec3(1.0));
	auto spIsotropic1 = CreateRef<Isotropic>(spColorTexture2, 5);
	m_spRayTracingScene->AddMaterial(spIsotropic1);

	auto spSphere4 = CreateRef<Sphere>(5);
	spSphere4->SetPosition(glm::dvec3(0));
	spSphere4->SetRadius(5000);
	spSphere4->SetMaterialID(5);
	auto spConstantMedium1 = CreateRef<ConstantMedium>(spSphere4, 0.0001);
	m_spObjectContainer->AddObject(spConstantMedium1);

	auto spLambertian1 = CreateRef<Lambertian>(6);
	auto spImageColorTexture = CreateRef<ImageColorTexture>("resources/texture/earthmap.jpg");
	spLambertian1->SetColorTexture(spImageColorTexture);
	m_spRayTracingScene->AddMaterial(spLambertian1);
	auto spSphere6 = CreateRef<Sphere>(6);
	spSphere6->SetPosition(glm::dvec3(400, 200, 400));
	spSphere6->SetRadius(100);
	spSphere6->SetMaterialID(6);
	m_spObjectContainer->AddObject(spSphere6);

	auto spLambertian2 = CreateRef<Lambertian>(7);
	auto spNoiseTexture = CreateRef<NoiseColorTexture>();
	spNoiseTexture->SetNoiseScale(0.1);
	spNoiseTexture->SetTurbDepth(7);
	spLambertian2->SetColorTexture(spNoiseTexture);
	m_spRayTracingScene->AddMaterial(spLambertian2);

	auto spSphere7 = CreateRef<Sphere>(7);
	spSphere7->SetPosition(glm::dvec3(220, 280, 300));
	spSphere7->SetRadius(80);
	spSphere7->SetMaterialID(7);
	m_spObjectContainer->AddObject(spSphere7);

	auto spLambertian3 = CreateRef<Lambertian>(8);
	spLambertian3->SetAlbedo(glm::dvec3(1.0));
	m_spRayTracingScene->AddMaterial(spLambertian3);

	int iEntityID(8); 
	auto spObjectContainer0 = CreateRef<ObjectContainer>();
	for (int i = 0; i < 1000; i++)
	{
		auto spSphereBox = CreateRef<Sphere>(iEntityID++);
		spSphereBox->SetPosition(glm::vec3(Random::Float(0, 165), Random::Float(0, 165), Random::Float(0, 165)));
		spSphereBox->SetRadius(10);
		spObjectContainer0->AddObject(spSphereBox);
	}
	auto spBoundingVolumeHierarchy0 = CreateRef<BoundingVolumeHierarchy>(spObjectContainer0, iEntityID++);
	spBoundingVolumeHierarchy0->SetMaterialID(8);
	spBoundingVolumeHierarchy0->SetTranslate(glm::dvec3(-100, 270, 395));
	spBoundingVolumeHierarchy0->SetRotateY(15);
	m_spObjectContainer->AddObject(spBoundingVolumeHierarchy0);

	auto spLambertian4 = CreateRef<Lambertian>(9);
	spLambertian4->SetAlbedo(glm::dvec3(0.48, 0.83, 0.53));
	m_spRayTracingScene->AddMaterial(spLambertian4);

	auto spObjectContainer1 = CreateRef<ObjectContainer>();
	const int boxes_per_side =20;
	for (int i = 0; i < boxes_per_side; i++) {
		for (int j = 0; j < boxes_per_side; j++) {
			auto w = 100.0;
			auto x0 = -1000.0 + i * w;
			auto z0 = -1000.0 + j * w;
			auto y0 = 0.0;
			auto x1 = x0 + w;
			auto y1 = Random::Float(1, 101);
			auto z1 = z0 + w;

			auto spBox = CreateRef<Box>(glm::dvec3(x0, y0, z0), glm::dvec3(x1, y1, z1), iEntityID++);
			spObjectContainer1->AddObject(spBox);
		}
	}
	auto spBoundingVolumeHierarchy1 = CreateRef<BoundingVolumeHierarchy>(spObjectContainer1, iEntityID++);
	spBoundingVolumeHierarchy1->SetMaterialID(9);
	m_spObjectContainer->AddObject(spBoundingVolumeHierarchy1);

	m_spRayTracingScene->SetObjectContainer(m_spObjectContainer);
}

