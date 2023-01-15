#pragma once
#include "entt.hpp"
#include "SandTable/Scene/Components.h"

SAND_TABLE_NAMESPACE_BEGIN
class Scene
{
public:
	Scene();
	~Scene();
	Ref<Entity> CreateEntity(const std::string& sName = std::string());
	void DestroyEntity(const Ref<Entity>& spEntity);

	void OnUpdate(const TimeStep& timeStep);
	void OnViewPortResize(unsigned int uiWidth, unsigned int uiHeight);
	const Ref<entt::registry>& Registry()const;

	Ref<Entity> GetPrimaryCameraEntity();
private:
	unsigned int m_uiWidth;
	unsigned int m_uiHeight;
	Ref<entt::registry> m_spRegistry;
};

SAND_TABLE_NAMESPACE_END

