#pragma once
#include "SandTable/System/PhysicsSystem2D.h"
#include "SandTable/Scene/Entity.h"

SAND_TABLE_NAMESPACE_BEGIN

enum class SceneState
{
	Play = 0,
	Edit,
	Simulate
};

class UUID;
class Scene :public std::enable_shared_from_this<Scene>
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

	void OnSimulationStart();
	void OnSimulationStop();

	void OnShowPhysicsCollider(const Ref<Camera>& spCamera);

	void OnUpdate(const TimeStep& timeStep);
	void OnUpdate(const Ref<Camera>& spCamera);
	void OnUpdate(const TimeStep& timeStep, const Ref<Camera>& spCamera);
	void OnViewPortResize(unsigned int uiWidth, unsigned int uiHeight);
	const Ref<entt::registry>& Registry()const;

	Ref<Entity> GetPrimaryCameraEntity();

private:
	void OnPhysics2DStart();
	void OnPhysics2DStop();

	void OnScriptStart();
	void OnScriptStop();

	void RenderScene(const Ref<Camera>& spCamera);
private:
	unsigned int m_uiWidth;
	unsigned int m_uiHeight;
	Ref<entt::registry> m_spRegistry;
	Ref<PhysicsSystem2D> m_spPhysicsSystem2D;
};

SAND_TABLE_NAMESPACE_END

