#include "SandBoxEditorLayer.h"
#include "ImGuizmo/ImGuizmo.h"

SAND_TABLE_NAMESPACE_BEGIN

extern const std::filesystem::path sAssetsDirector;

SandBoxEditorLayer::SandBoxEditorLayer() 
	:m_vec4Color(glm::vec4(0.2f, 0.3f, 0.8f, 1.0f)),
	m_bViewportHovered(true),
	m_iGizmoType(-1)
{
	m_spOrthoGraphicCameraController = CreateRef<OrthoGraphicCameraController>
		(Application::GetApplication()->GetWindowWidth(), Application::GetApplication()->GetWindowHeight());

	m_spScene = CreateRef<Scene>();
	m_spSceneHierarchyPanel = CreateRef<SceneHierarchyPanel>(m_spScene);
	m_spContentBrowserPanel = CreateRef<ContentBrowserPanel>();
	m_spSceneSerializer = CreateRef<SceneSerializer>(m_spScene);
}

SandBoxEditorLayer::~SandBoxEditorLayer()
{
}

void SandBoxEditorLayer::OnAttach()
{
	m_spTexture = Texture2D::Create("assets/textures/Checkerboard.png");
	m_spTextureStar = Texture2D::Create("assets/textures/Star.png");

	m_spTextureSprite = Texture2D::Create("assets/games/textures/RPGpack_sheet_2X.png");
	m_spSubTexStairs = CreateRef<SubTexture2D>(m_spTextureSprite, glm::vec2(7, 6), glm::vec2(128, 128));
	m_spSubTexBarrel = CreateRef<SubTexture2D>(m_spTextureSprite, glm::vec2(1, 11), glm::vec2(128, 128));
	m_spSubTexTree = CreateRef<SubTexture2D>(m_spTextureSprite, glm::vec2(2, 1), glm::vec2(128, 128), glm::vec2(1.f, 2.f));

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

	m_spSquareEntity = m_spScene->CreateEntity("Square Entity");
	m_spSquareEntity->AddComponent<SpriteRenderComponent>(glm::vec4(0.f, 1.f, 0.f, 1.f));

	m_spCameraEntity = m_spScene->CreateEntity("Camera Entity");
	m_spCameraEntity->AddComponent<CameraComponent>(m_spOrthoGraphicCameraController->GetCamera());

	//class CameraController :public ScriptableEntity
	//{
	//public:
	//	virtual void OnCreate()
	//	{
	//		LOG_DEV_INFO("On Create");
	//	}

	//	virtual void OnUpdate(TimeStep ts)
	//	{
	//		auto& translation = GetComponent<TransformComponent>().Translation;
	//		float speed = 5.f;
	//		if (Input::IsKeyPressed(Key::A))
	//		{
	//			translation.x -= speed * ts;
	//		}
	//		if (Input::IsKeyPressed(Key::D))
	//		{
	//			translation.x += speed * ts;
	//		}
	//		if (Input::IsKeyPressed(Key::W))
	//		{
	//			translation.y += speed * ts;
	//		}
	//		if (Input::IsKeyPressed(Key::S))
	//		{
	//			translation.y -= speed * ts;
	//		}
	//	}
	//};
	//m_spCameraEntity->AddComponent<NativeScriptComponent>().Bind<CameraController>();

}

void SandBoxEditorLayer::OnDetach()
{

}

void SandBoxEditorLayer::OnUpdate(const TimeStep& timeStep)
{
	SAND_TABLE_PROFILE_SCOPE("SandBoxEditorLayer::OnUpdate");

	auto spFrameBuffer = std::dynamic_pointer_cast<FrameBuffer>(m_spFrameBuffer);
	SAND_TABLE_ASSERT(spFrameBuffer, "FrameBuffer is null in Edit Layer");
	auto spSpecification = spFrameBuffer->GetFrameBufferSpecification();
	spFrameBuffer->Resize(m_vec2RenderViewPortSize.x, m_vec2RenderViewPortSize.y);

	//if (m_vec2RenderViewPortSize.x > 0 && m_vec2RenderViewPortSize.y > 0)
	//{
	//	m_spOrthoGraphicCameraController->OnResize(m_vec2RenderViewPortSize.x, m_vec2RenderViewPortSize.y);
	//}

	//if(m_bRenderWindowActive)
	//{
	//	SAND_TABLE_PROFILE_SCOPE("CameraController::OnUpdate");
	//	m_spOrthoGraphicCameraController->OnUpdate(timeStep);
	//}

	Render2D::ResetStats();
	spFrameBuffer->Bind();
	RenderCommand::SetClearColor(glm::vec4(glm::vec3(0.1f), 1.0f));
	RenderCommand::Clear();
	spFrameBuffer->ClearColorAttachment(1, -1);

	m_spScene->OnViewPortResize(m_vec2RenderViewPortSize.x, m_vec2RenderViewPortSize.y);
	m_spScene->OnUpdate(timeStep);

	ImVec2 vec2MousePos = ImGui::GetMousePos();
	vec2MousePos.x -= m_vec2RenderViewPortBounds[0].x;
	vec2MousePos.y = m_vec2RenderViewPortSize.y - (vec2MousePos.y - m_vec2RenderViewPortBounds[0].y);

	if (vec2MousePos.x >= 0 && vec2MousePos.y >= 0 &&
		vec2MousePos.x < m_vec2RenderViewPortSize.x && vec2MousePos.y < m_vec2RenderViewPortSize.y)
	{
		int iPixelData = spFrameBuffer->ReadPixel
		(1, static_cast<unsigned int>(vec2MousePos.x), static_cast<unsigned int>(vec2MousePos.y));

		m_spHoveredEntity = iPixelData != -1 ? CreateRef<Entity>(m_spScene->Registry(), iPixelData) : nullptr;
	}

	spFrameBuffer->UnBind();
}

void SandBoxEditorLayer::OnImGuiRender()
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
			if (ImGui::MenuItem("New", "Ctrl+O"))
			{
				NewScene();
			}
			if (ImGui::MenuItem("Open...","Ctrl+O"))
			{
				OpenScene();
			}
			if (ImGui::MenuItem("Save As...","Ctrl+Shift+S"))
			{
				SaveSceneAs();
			}
			if (ImGui::MenuItem("Exit")) { Application::GetApplication()->Close(); }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::End();

	m_spSceneHierarchyPanel->OnImGuiRender();
	m_spContentBrowserPanel->OnImGuiRender();

	ImGui::Begin("Stats");
	std::string sEntityName("None");
	if (m_spHoveredEntity != nullptr)
	{
		sEntityName = m_spHoveredEntity->GetComponent<TagComponent>().Tag;
	}
	ImGui::Text("Hovered Entity: %s", sEntityName.c_str());
	auto stats = Render2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Draw Quads: %d", stats.QuadCount);
	ImGui::Text("Draw Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Draw Indices: %d", stats.GetTotalIndexCount());

	//{
	//	auto& vec4Pan = m_spCameraEntity->GetComponent<TransformComponent>().Translation;
	//	if (ImGui::DragFloat3("Camera Transform", glm::value_ptr(vec4Pan)))
	//	{
	//		auto& spCamera = m_spCameraEntity->GetComponent<CameraComponent>().Camera;
	//		spCamera->SetPosition(vec4Pan);
	//	}
	//}

	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("viewport");

	ImVec2 vec2MinRegion = ImGui::GetWindowContentRegionMin();
	ImVec2 vec2MaxRegion = ImGui::GetWindowContentRegionMax();
	auto vec2WindowPos = ImGui::GetWindowPos();
	m_vec2RenderViewPortBounds[0] = { vec2MinRegion.x + vec2WindowPos.x, vec2MinRegion.y + vec2WindowPos.y };
	m_vec2RenderViewPortBounds[1] = { vec2MaxRegion.x + vec2WindowPos.x, vec2MaxRegion.y + vec2WindowPos.y };

	m_bViewportHovered = ImGui::IsWindowHovered();
	m_bViewportFocused = ImGui::IsWindowFocused();
	Application::GetApplication()->BlockEvents(!m_bViewportHovered);

	m_vec2RenderViewPortSize = ImGui::GetContentRegionAvail();
	auto spFrameBuffer = std::dynamic_pointer_cast<FrameBuffer>(m_spFrameBuffer);
	SAND_TABLE_ASSERT(spFrameBuffer, "FrameBuffer is null in Edit Layer");
	auto uiTextureID = spFrameBuffer->GetColorAttachment();
	ImGui::Image((void*)uiTextureID, m_vec2RenderViewPortSize, ImVec2(0, 1), ImVec2(1, 0));

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
		{
			const wchar_t* path = (const wchar_t*)payload->Data;
			OpenScene(std::filesystem::path(sAssetsDirector) / path);
		}
		ImGui::EndDragDropTarget();
	}

	//Gizmos
	auto spSelectedEntity = m_spSceneHierarchyPanel->GetSelectedEntity();
	if (spSelectedEntity != nullptr && m_iGizmoType != -1)
	{
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		auto windowWidth = ImGui::GetWindowWidth();
		auto windowHeight = ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,
			windowWidth, windowHeight);

		auto cameraEntity = m_spScene->GetPrimaryCameraEntity();
		auto spCamera = cameraEntity->GetComponent<CameraComponent>().GetCamera();
		auto cameraProjection = spCamera->GetProjectionMatrix();
		glm::mat4 cameraView = cameraEntity->GetComponent<TransformComponent>().GetTransform();

		auto& transformComponent = spSelectedEntity->GetComponent<TransformComponent>();
		auto transfrom = transformComponent.GetTransform();

		//snapping
		bool snap = Input::IsKeyPressed(Key::LeftControl);
		float snapValue = 0.5f;
		if (static_cast<ImGuizmo::OPERATION>(m_iGizmoType) == ImGuizmo::OPERATION::ROTATE)
		{
			snapValue = 45.f;
		}
		glm::vec3 snapValues(snapValue);

		ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
			static_cast<ImGuizmo::OPERATION>(m_iGizmoType), ImGuizmo::LOCAL, glm::value_ptr(transfrom),
			nullptr, snap ? glm::value_ptr(snapValues) : nullptr);

		if (ImGuizmo::IsUsing())
		{
			glm::vec3 translation, rotation, scale;
			MathUtils::DecomposeTransform(transfrom, translation, rotation, scale);
			glm::vec3 deltaRotation = rotation - transformComponent.Rotation;

			transformComponent.Translation = translation;
			transformComponent.Rotation += deltaRotation;
			transformComponent.Scale = scale;
		}
	}


	ImGui::End();
	ImGui::PopStyleVar();
}

void SandBoxEditorLayer::OnEvent(Event& e)
{
	m_spOrthoGraphicCameraController->OnEvent(e);

	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUN(SandBoxEditorLayer::OnKeyPressed));
	dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUN(SandBoxEditorLayer::OnMousePressed));
}

bool SandBoxEditorLayer::OnKeyPressed(KeyPressedEvent& e)
{
	if (e.IsRepeat())
	{
		return false;
	}

	bool bControl = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
	bool bShift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

	switch (e.GetKeyCode())
	{
	case Key::N:
	{
		if (bControl)
		{
			NewScene();
		}
		break;
	}
	case Key::O:
	{
		if (bControl)
		{
			OpenScene();
		}
		break;
	}
	case Key::S:
	{
		if (bControl && bShift)
		{
			SaveSceneAs();
		}
		break;
	}
	case Key::Q:
	{
		m_iGizmoType = -1;
		break;
	}
	case Key::W:
	{
		m_iGizmoType = ImGuizmo::OPERATION::TRANSLATE;
		break;
	}
	case Key::E:
	{
		m_iGizmoType = ImGuizmo::OPERATION::ROTATE;
		break;
	}
	case Key::R:
	{
		m_iGizmoType = ImGuizmo::OPERATION::SCALE;
		break;
	}
	default:
		break;
	}
	return true;
}

bool SandBoxEditorLayer::OnMousePressed(MouseButtonPressedEvent& e)
{
	if (e.GetMouseButton() == Mouse::ButtonLeft && m_bViewportHovered
		&& !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
	{
		m_spSceneHierarchyPanel->SetSelectedEntity(m_spHoveredEntity);
	}

	return false;
}

void SandBoxEditorLayer::NewScene()
{
	m_spScene = CreateRef<Scene>();
	m_spScene->OnViewPortResize(m_vec2RenderViewPortSize.x, m_vec2RenderViewPortSize.y);
	m_spSceneHierarchyPanel->SetSelectedScene(m_spScene);
}

void SandBoxEditorLayer::OpenScene()
{
	auto sFilePath = PlatformUtils::OpenFile("SandTable Scene (*.scene)\0*.scene\0");
	if (!sFilePath.empty())
	{
		OpenScene(sFilePath);
	}
}

void SandBoxEditorLayer::OpenScene(const std::filesystem::path& path)
{
	m_spScene = CreateRef<Scene>();
	m_spScene->OnViewPortResize(m_vec2RenderViewPortSize.x, m_vec2RenderViewPortSize.y);
	m_spSceneHierarchyPanel->SetSelectedScene(m_spScene);
	m_spSceneSerializer->SetSelectedScene(m_spScene);
	m_spSceneSerializer->DeSerialize(path.string());
}

void SandBoxEditorLayer::SaveSceneAs()
{
	auto sFilePath = PlatformUtils::SaveFile("SandTable Scene (*.scene)\0*.scene\0");
	if (!sFilePath.empty())
	{
		m_spSceneSerializer->Serialize(sFilePath);
	}
}

SAND_TABLE_NAMESPACE_END
