#pragma once
#include "SandTable.h"
#include <imgui/imgui.h>
#include "SandTable/Scene/SceneSerializer.h"

SAND_TABLE_NAMESPACE_BEGIN

class SandBoxEditorLayer:public Layer
{
public:
	SandBoxEditorLayer();
	~SandBoxEditorLayer();
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(const TimeStep& timeStep) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Event& e) override;
private:
	bool OnKeyPressed(KeyPressedEvent& e);
	bool OnMousePressed(MouseButtonPressedEvent& e);
	void NewScene();
	void OpenScene();
	void OpenScene(const std::filesystem::path& path);
	void SaveSceneAs();
	void SaveScene();
	void OnScenePlay();
	void OnSceneStop();

	void UIToolbar();
	void OnDuplicateEntity();
private:
	Ref<Texture> m_spIconPlay;
	Ref<Texture> m_spIconStop;

	glm::vec4 m_vec4Color;

	Ref<Buffer> m_spFrameBuffer;

	bool m_bViewportHovered;
	bool m_bViewportFocused;
	ImVec2 m_vec2RenderViewPortSize;
	std::array<ImVec2, 2> m_vec2RenderViewPortBounds;

	Ref<Scene> m_spActiveScene;
	Ref<Scene> m_spEditorScene;
	std::filesystem::path m_sEditorScenePath;

	Ref<Entity> m_spSquareEntity;
	Ref<Entity> m_spCameraEntity;
	Ref<Entity> m_spHoveredEntity;

	//panel
	int m_iGizmoType;
	Ref<SceneHierarchyPanel> m_spSceneHierarchyPanel;
	Ref<ContentBrowserPanel> m_spContentBrowserPanel;

	Ref<SceneSerializer> m_spSceneSerializer;

	SceneState m_eSceneState;

	Ref<Camera> m_spEditCamera;
};

SAND_TABLE_NAMESPACE_END

