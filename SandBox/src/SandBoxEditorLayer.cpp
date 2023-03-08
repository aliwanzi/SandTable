#include "SandBoxEditorLayer.h"
#include "ImGuizmo/ImGuizmo.h"

SAND_TABLE_NAMESPACE_BEGIN

extern const std::filesystem::path sAssetsDirector;

SandBoxEditorLayer::SandBoxEditorLayer()
	:m_vec4Color(glm::vec4(0.2f, 0.3f, 0.8f, 1.0f)),
	m_bViewportHovered(false),
	m_bViewportFocused(false),
	m_iGizmoType(-1),
	m_eSceneState(SceneState::Edit),
	m_bShowPhysicsCollider(false),
	m_bShowDemoWindow(false)
{
	auto uiWindowWidth = Application::GetApplication()->GetWindowWidth();
	auto uiWindowHeight = Application::GetApplication()->GetWindowHeight();
	m_spEditCamera = CreateRef<EditorCamera>(30.f, static_cast<float>(uiWindowWidth) / static_cast<float>(uiWindowHeight));

	m_spEditorScene = CreateRef<Scene>();
	m_spSceneHierarchyPanel = CreateRef<SceneHierarchyPanel>(m_spEditorScene);
	m_spContentBrowserPanel = CreateRef<ContentBrowserPanel>();
	m_spSceneSerializer = CreateRef<SceneSerializer>(m_spEditorScene);

	std::vector<FrameBufferTextureSpecification> vecFrameBufferTextureSpecification =
	{
		FramebufferTextureFormat::RGBA8,
		FramebufferTextureFormat::RED_INTERGER,
		FramebufferTextureFormat::DEPTH24_STENCIL8
	};
	auto spFrameBufferAttachmentSpecification = CreateRef<FrameBufferAttachmentSpecification>(vecFrameBufferTextureSpecification);
	auto spFrameBufferSpecification = CreateRef<FrameBufferSpecification>
		(uiWindowHeight, uiWindowHeight, spFrameBufferAttachmentSpecification);

	m_spFrameBuffer = FrameBuffer::Create(spFrameBufferSpecification);

	m_spIconPlay = Texture2D::Create("assets/textures/Icons/PlayButton.png");
	m_spIcomSimulate = Texture2D::Create("assets/textures/Icons/SimulateButton.png");
	m_spIconStop = Texture2D::Create("assets/textures/Icons/StopButton.png");
}

SandBoxEditorLayer::~SandBoxEditorLayer()
{
}

void SandBoxEditorLayer::OnAttach()
{
	m_spCameraEntity = m_spEditorScene->CreateEntity("Camera Entity");
	m_spCameraEntity->AddComponent<CameraComponent>();

	m_spSquareEntity = m_spEditorScene->CreateEntity("Square Entity");
	m_spSquareEntity->AddComponent<SpriteRenderComponent>();

	m_spCameraEntity = m_spEditorScene->CreateEntity("Circle Entity");
	m_spCameraEntity->AddComponent<CircleRenderComponent>();

	auto commandLineArgs = Application::GetApplication()->GetApplicationSpecification()->CommandLineArgs;
	if (commandLineArgs.Count>1)
	{
		OpenScene(commandLineArgs[1]);
	}
}

void SandBoxEditorLayer::OnDetach()
{

}

void SandBoxEditorLayer::OnUpdate(const TimeStep& timeStep)
{
	SAND_TABLE_PROFILE_SCOPE("SandBoxEditorLayer::OnUpdate");
	m_spEditorScene->OnViewPortResize(m_vec2RenderViewPortSize.x, m_vec2RenderViewPortSize.y);

	auto spFrameBuffer = std::dynamic_pointer_cast<FrameBuffer>(m_spFrameBuffer);
	SAND_TABLE_ASSERT(spFrameBuffer, "FrameBuffer is null in Edit Layer");
	spFrameBuffer->Resize(m_vec2RenderViewPortSize.x, m_vec2RenderViewPortSize.y);
	m_spEditCamera->SetViewPortSize(m_vec2RenderViewPortSize.x, m_vec2RenderViewPortSize.y);

	Render2D::ResetStats();
	spFrameBuffer->Bind();
	RenderCommand::SetClearColor(glm::vec4(glm::vec3(0.1f), 1.0f));
	RenderCommand::Clear();
	spFrameBuffer->ClearColorAttachment(1, -1);

	switch (m_eSceneState)
	{
		case SceneState::Play:
		{
			m_spActiveScene->OnUpdate(timeStep);
			break;
		}
		case SceneState::Edit:
		{
			auto spEditorCamera = std::dynamic_pointer_cast<EditorCamera>(m_spEditCamera);
			spEditorCamera->OnUpdate(timeStep);
			m_spEditorScene->OnUpdate(spEditorCamera);
			break;
		}
		case SceneState::Simulate:
		{
			auto spEditorCamera = std::dynamic_pointer_cast<EditorCamera>(m_spEditCamera);
			m_spActiveScene->OnUpdate(timeStep, spEditorCamera);
			break;
		}
	}

	ImVec2 vec2MousePos = ImGui::GetMousePos();
	vec2MousePos.x -= m_vec2RenderViewPortBounds[0].x;
	vec2MousePos.y = m_vec2RenderViewPortSize.y - (vec2MousePos.y - m_vec2RenderViewPortBounds[0].y);

	if (vec2MousePos.x >= 0 && vec2MousePos.y >= 0 &&
		vec2MousePos.x < m_vec2RenderViewPortSize.x && vec2MousePos.y < m_vec2RenderViewPortSize.y)
	{
		int iPixelData = spFrameBuffer->ReadPixel
		(1, static_cast<unsigned int>(vec2MousePos.x), static_cast<unsigned int>(vec2MousePos.y));
		if (iPixelData != -1)
		{
			m_spHoveredEntity = CreateRef<Entity>(m_spEditorScene->Registry(), iPixelData);
		}
		else
		{
			m_spHoveredEntity = nullptr;
		}
	}

	OnOverlayRender();

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

		if (ImGui::BeginMenu("Script"))
		{
			if (ImGui::MenuItem("Reload assembly","Ctrl+R"))
			{
				ScriptEngine::ReloadAssembly();
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	m_spSceneHierarchyPanel->OnImGuiRender();
	m_spContentBrowserPanel->OnImGuiRender();

	ImGui::Begin("Stats");
	std::string sEntityName("None");
	if (m_spHoveredEntity != nullptr)
	{
		sEntityName = m_spHoveredEntity->GetComponent<TagComponent>().Tag;
	}
	ImGui::Text("Hovered Entity: %s", sEntityName.c_str());


	ImGui::Text("Renderer2D Stats:");
	ImGui::Columns(5, "Renderer2D Stats"); // 4-ways, with border
	ImGui::Separator();
	ImGui::Text("Draw Primitive"); ImGui::NextColumn();
	ImGui::Text("Calls"); ImGui::NextColumn();
	ImGui::Text("Count"); ImGui::NextColumn();
	ImGui::Text("Vertices"); ImGui::NextColumn();
	ImGui::Text("Indices"); ImGui::NextColumn();
	ImGui::Separator();
	ImGui::Text("Quad"); ImGui::NextColumn();
	auto spQuadStatic = Render2D::GetQuadStatic();
	ImGui::Text("%d", spQuadStatic->GetDrawCalls()); ImGui::NextColumn();
	ImGui::Text("%d", spQuadStatic->GetDrawCount()); ImGui::NextColumn();
	ImGui::Text("%d", spQuadStatic->GetTotalVertexCount()); ImGui::NextColumn();
	ImGui::Text("%d", spQuadStatic->GetTotalIndexCount()); ImGui::NextColumn();
	ImGui::Separator();
	ImGui::Text("Circle"); ImGui::NextColumn();
	auto spCircleStatic = Render2D::GetCircleStatic();
	ImGui::Text("%d", spCircleStatic->GetDrawCalls()); ImGui::NextColumn();
	ImGui::Text("%d", spCircleStatic->GetDrawCount()); ImGui::NextColumn();
	ImGui::Text("%d", spCircleStatic->GetTotalVertexCount()); ImGui::NextColumn();
	ImGui::Text("%d", spCircleStatic->GetTotalIndexCount()); ImGui::NextColumn();
	ImGui::Columns(1);
	ImGui::Separator();
	ImGui::End();

	ImGui::Begin("Setting");
	ImGui::Checkbox("Show Physics Collider", &m_bShowPhysicsCollider);
	ImGui::Checkbox("Show Demo Window", &m_bShowDemoWindow);
	if (m_bShowDemoWindow)
	{
		ImGui::ShowDemoWindow(&m_bShowDemoWindow);
	}
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
	if (spSelectedEntity != nullptr && m_iGizmoType != -1 && m_eSceneState == SceneState::Edit)
	{
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		auto windowWidth = ImGui::GetWindowWidth();
		auto windowHeight = ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,
			windowWidth, windowHeight);

		const glm::mat4& cameraProjection = m_spEditCamera->GetProjectionMatrix();
		const glm::mat4& cameraView = m_spEditCamera->GetViewMatrix();

		// Entity transform
		auto& transformComponent = spSelectedEntity->GetComponent<TransformComponent>();
		glm::mat4 transform = transformComponent.GetTransform();

		//snapping
		bool snap = Input::IsKeyPressed(Key::LeftControl);
		float snapValue = 0.5f;
		if (static_cast<ImGuizmo::OPERATION>(m_iGizmoType) == ImGuizmo::OPERATION::ROTATE)
		{
			snapValue = 45.f;
		}
		glm::vec3 snapValues(snapValue);

		ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
			static_cast<ImGuizmo::OPERATION>(m_iGizmoType), ImGuizmo::LOCAL, glm::value_ptr(transform),
			nullptr, snap ? glm::value_ptr(snapValues) : nullptr);

		if (ImGuizmo::IsUsing())
		{
			glm::vec3 translation, rotation, scale;
			MathUtils::DecomposeTransform(transform, translation, rotation, scale);

			glm::vec3 deltaRotation = rotation - transformComponent.Rotation;
			transformComponent.Translation = translation;
			transformComponent.Rotation += deltaRotation;
			transformComponent.Scale = scale;
		}
	}

	ImGui::End();
	ImGui::PopStyleVar();

	UIToolbar();

	ImGui::End();
}

void SandBoxEditorLayer::OnEvent(Event& e)
{
	if (m_eSceneState == SceneState::Edit)
	{
		auto spEditorCamera = std::dynamic_pointer_cast<EditorCamera>(m_spEditCamera);
		spEditorCamera->OnEvent(e);
	}

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
		if (bControl)
		{
			bShift ? SaveSceneAs() : SaveScene();
		}
		break;
	}
	case Key::D:
	{
		if (bControl)
		{
			OnDuplicateEntity();
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
	m_spEditorScene = CreateRef<Scene>();
	m_spEditorScene->OnViewPortResize(m_vec2RenderViewPortSize.x, m_vec2RenderViewPortSize.y);
	m_spSceneHierarchyPanel->SetSelectedScene(m_spEditorScene);

	m_sEditorScenePath = std::filesystem::path();
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
	if (m_eSceneState != SceneState::Edit)
	{
		OnSceneStop();
	}

	if (path.extension().string()!=".scene")
	{
		LOG_DEV_WARN("Could not load {0} - not a scene file", path.filename().string());
		return;
	}

	NewScene();
	m_spSceneSerializer->SetSelectedScene(m_spEditorScene);
	m_spSceneSerializer->DeSerialize(path.string());
	m_sEditorScenePath = path;
}

void SandBoxEditorLayer::SaveSceneAs()
{
	auto sFilePath = PlatformUtils::SaveFile("SandTable Scene (*.scene)\0*.scene\0");
	if (!sFilePath.empty())
	{
		m_spSceneSerializer->Serialize(sFilePath);
		m_sEditorScenePath = sFilePath;
	}
}

void SandBoxEditorLayer::SaveScene()
{
	if (!m_sEditorScenePath.empty())
	{
		m_spSceneSerializer->Serialize(m_sEditorScenePath.string());
	}
}

void SandBoxEditorLayer::OnScenePlay()
{
	m_eSceneState = SceneState::Play;

	m_spActiveScene = CreateRef<Scene>(m_spEditorScene);
	m_spActiveScene->OnRuntimeStart();

	m_spSceneHierarchyPanel->SetSelectedScene(m_spActiveScene);
}

void SandBoxEditorLayer::OnSceneSimulate()
{
	m_eSceneState = SceneState::Simulate;
	m_spActiveScene = CreateRef<Scene>(m_spEditorScene);
	m_spActiveScene->OnSimulationStart();
}

void SandBoxEditorLayer::OnSceneStop()
{
	switch (m_eSceneState)
	{
		case SandTable::SceneState::Play:
		{
			m_spActiveScene->OnRuntimeStop();
			break;
		}
		case SandTable::SceneState::Simulate:
		{
			m_spActiveScene->OnSimulationStop();
			break;
		}
	}
	m_eSceneState = SceneState::Edit;

}

void SandBoxEditorLayer::OnDuplicateEntity()
{
	if (m_eSceneState == SceneState::Edit)
	{
		auto spEntity = m_spSceneHierarchyPanel->GetSelectedEntity();
		m_spEditorScene->CreateEntity(spEntity);
	}
}

void SandBoxEditorLayer::OnOverlayRender()
{
	if (!m_bShowPhysicsCollider)
	{
		return;
	}

	switch (m_eSceneState)
	{
		case SandTable::SceneState::Play:
		{
			if (m_spActiveScene != nullptr)
			{
				auto spCameraEntity = m_spActiveScene->GetPrimaryCameraEntity();
				auto spCamera = spCameraEntity->GetComponent<CameraComponent>().GetCamera();
				m_spActiveScene->OnShowPhysicsCollider(spCamera);
			}
			break;
		}
		case SandTable::SceneState::Edit:
		{
			if (m_spEditorScene != nullptr)
			{
				m_spEditorScene->OnShowPhysicsCollider(m_spEditCamera);
			}
			break;
		}
	}
}


void SandBoxEditorLayer::UIToolbar()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

	auto& colors = ImGui::GetStyle().Colors;
	const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
	const auto& buttonActive = colors[ImGuiCol_ButtonActive];
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

	ImGui::Begin("#toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	
	float size = ImGui::GetWindowHeight() - 4.0f;
	ImVec4 tintColor = ImVec4(1, 1, 1, 1);
	{
		auto spIconTexture = (m_eSceneState == SceneState::Edit || m_eSceneState == SceneState::Simulate)
			? m_spIconPlay : m_spIconStop;
		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x * 0.5f - size * 0.5f);
		if (ImGui::ImageButton((ImTextureID)(uint64_t)spIconTexture->GetRenderID(), ImVec2(size, size),
			ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor))
		{
			switch (m_eSceneState)
			{
			case SandTable::SceneState::Play:
			{
				OnSceneStop();
				break;
			}
			case SandTable::SceneState::Edit:
			case SceneState::Simulate:
			{
				OnScenePlay();
				break;
			}
			default:
				break;
			}
		}
	}
	ImGui::SameLine();
	{
		auto spIconTexture = (m_eSceneState == SceneState::Edit || m_eSceneState == SceneState::Play)
			? m_spIcomSimulate : m_spIconStop;
		//ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x * 0.5f - size * 0.5f);
		if (ImGui::ImageButton((ImTextureID)(uint64_t)spIconTexture->GetRenderID(), ImVec2(size, size),
			ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor))
		{
			switch (m_eSceneState)
			{
			case SceneState::Simulate:
			{
				OnSceneStop();
				break;
			}
			case SceneState::Edit:
			case SceneState::Play:
			{
				OnSceneSimulate();
				break;
			}
			default:
				break;
			}
		}
	}

	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(3);
	ImGui::End();

}
SAND_TABLE_NAMESPACE_END
