#pragma once

SAND_TABLE_NAMESPACE_BEGIN
class Scene;
class Entity;
class SceneHierarchyPanel
{
public:
	SceneHierarchyPanel(const Ref<Scene>& spScene);
	void OnImGuiRender();
	void SetSelectedEntity(const Ref<Entity>& spEntity);
private:
	void DrawEntityNode(const Ref<Entity>& spEntity);
	void DrawComponents(const Ref<Entity>& spEntity);
private:
	Ref<Scene> m_spScene;
	Ref<Entity> m_spSelectedEntity;
};
SAND_TABLE_NAMESPACE_END


