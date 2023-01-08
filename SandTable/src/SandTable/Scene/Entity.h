#pragma once
#include "entt.hpp"
SAND_TABLE_NAMESPACE_BEGIN

class Entity
{
public:
	Entity();
	Entity(const Ref<entt::registry>& spRegistry);
	Entity(const Ref<entt::registry>& spRegistry, const Ref<entt::entity>& spEntity);


	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		SAND_TABLE_ASSERT(!HasComponent<T>(), "Entity already has component!");
		return m_spRegistry->emplace<T>(*m_spEntity, std::forward<Args>(args)...);
	}

	template<typename T>
	T& GetComponent()
	{
		SAND_TABLE_ASSERT(HasComponent<T>(), "Entity does not has component!");
		return m_spRegistry->get<T>(*m_spEntity);
	}

	template<typename T>
	bool HasComponent()
	{
		return m_spRegistry->has<T>(*m_spEntity);
	}

	template<typename T>
	void RemoveComponent()
	{
		m_spRegistry->remove<T>(*m_spEntity);
	}

	operator uint32_t() const;

	bool operator==(const Ref<Entity> spEntity) const;
	bool operator!=(const Ref<Entity> spEntity) const;
private:
	Ref<entt::registry> m_spRegistry;
	Ref<entt::entity> m_spEntity;
};

SAND_TABLE_NAMESPACE_END

