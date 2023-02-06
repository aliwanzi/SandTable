#pragma once
#include "SandTable/System/PhysicsSystem2D.h"
#include "SandTable/Scene/Entity.h"

SAND_TABLE_NAMESPACE_BEGIN

enum class SceneState
{
	Play = 0,
	Edit,
};

class UUID;
class Scene
{
public:
	Scene();
	~Scene();
	Scene(const Ref<Scene>& spScene);

	Ref<Entity> CreateEntity(const Ref<Entity>& spSrcEntity);
	Ref<Entity> CreateEntity(const std::string& sName = std::string());
	Ref<Entity> CreateEntityWithUUID(const UUID& uuid, const std::string& sName);

	void OnRuntimeStart();
	void OnRuntimeStop();

	void OnUpdate(const TimeStep& timeStep);
	void OnUpdate(const Ref<Camera>& spCamera);
	void OnViewPortResize(unsigned int uiWidth, unsigned int uiHeight);
	const Ref<entt::registry>& Registry()const;

	Ref<Entity> GetPrimaryCameraEntity();

private:
	
	void RenderScene(const Ref<Camera>& spCamera);
private:
	unsigned int m_uiWidth;
	unsigned int m_uiHeight;
	Ref<entt::registry> m_spRegistry;
	Ref<PhysicsSystem2D> m_spPhysicsSystem2D;
};

SAND_TABLE_NAMESPACE_END

