#include "pch.h"
#include "Scene.h"

SAND_TABLE_NAMESPACE_BEGIN
Scene::Scene()
{
	m_spRegistry = CreateRef<entt::registry>();
	entt::entity entity = m_spRegistry->create();
}

Scene::~Scene()
{

}

SAND_TABLE_NAMESPACE_END
