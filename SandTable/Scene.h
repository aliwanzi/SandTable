#pragma once
#include "entt.hpp"

SAND_TABLE_NAMESPACE_BEGIN

class Scene
{
public:
	Scene();
	~Scene();
private:
	Ref<entt::registry> m_spRegistry;
};

SAND_TABLE_NAMESPACE_END

