#pragma once
#include "entt.hpp"
#include "SandTable/Scene/Components.h"

SAND_TABLE_NAMESPACE_BEGIN

class Entity
{
public:
	Entity(const Ref<entt::registry>& spRegistry);
	Entity(const Ref<entt::registry>& spRegistry, const entt::entity& Entity);
	Entity(const Ref<entt::registry>& spRegistry, int iEntityID);
	void Destrory();

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		SAND_TABLE_ASSERT(!HasComponent<T>(), "Entity already has component!");
		return m_spRegistry->emplace<T>(m_entity, std::forward<Args>(args)...);
	}

	template<typename T, typename... Args>
	T& AddOrReplaceComponent(Args&&... args)
	{
		return m_spRegistry->emplace_or_replace<T>(m_entity, std::forward<Args>(args)...);
	}

	template<typename T>
	T& GetComponent()
	{
		SAND_TABLE_ASSERT(HasComponent<T>(), "Entity does not has component!");
		return m_spRegistry->get<T>(m_entity);
	}

	template<typename T>
	bool HasComponent()
	{
		return m_spRegistry->has<T>(m_entity);
	}

	template<typename T>
	void RemoveComponent()
	{
		m_spRegistry->remove<T>(m_entity);
	}

	bool operator==(const Entity& spEntity) const;
	bool operator!=(const Entity& spEntity) const;
	Entity& operator=(const Entity& spEntity);
	Ref<Entity> operator=(const Ref<Entity>& spEntity);

	UUID GetUUID();
	const std::string& GetName();

	operator uint32_t() const;
	operator entt::entity()const;

private:
	Ref<entt::registry> m_spRegistry;
	entt::entity m_entity;
};

SAND_TABLE_NAMESPACE_END

