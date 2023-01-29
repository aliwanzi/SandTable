#pragma once
#include "SandTable/System/PhysicsSystem2D.h"

SAND_TABLE_NAMESPACE_BEGIN

enum class SceneState
{
	PLAY = 0,
	STOP,
};

//class b2World;
class Scene
{
public:
	Scene();
	~Scene();
	Ref<Entity> CreateEntity(const std::string& sName = std::string());
	void DestroyEntity(const Ref<Entity>& spEntity);

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

