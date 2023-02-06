#include "pch.h"
#include "Entity.h"

SAND_TABLE_NAMESPACE_BEGIN

Entity::Entity(const Ref<entt::registry>& spRegistry):
	m_spRegistry(spRegistry)
{
	m_entity = m_spRegistry->create();
}

Entity::Entity(const Ref<entt::registry>& spRegistry, const entt::entity& Entity):
	m_spRegistry(spRegistry),m_entity(Entity)
{

}

Entity::Entity(const Ref<entt::registry>& spRegistry, int iEntityID) :
	m_spRegistry(spRegistry)
{
	m_entity = static_cast<entt::entity>(iEntityID);
}

void Entity::Destrory()
{
	m_spRegistry->destroy(m_entity);
}

bool Entity::operator==(const Entity& entity) const
{
	return m_entity == entity.m_entity;
}

bool Entity::operator!=(const Entity& entity) const
{
	return !(*this == entity);
}

Entity& Entity::operator=(const Entity& spEntity)
{
	m_entity = spEntity.m_entity;
	m_spRegistry = spEntity.m_spRegistry;
	return *this;
}

Ref<Entity>& Entity::operator=(const Ref<Entity>& spEntity)
{
	m_entity = spEntity->m_entity;
	m_spRegistry = spEntity->m_spRegistry;
	return Ref<Entity>(this);
}

UUID Entity::GetUUID()
{
	return GetComponent<IDComponent>().ID;
}

const std::string& Entity::GetName()
{
	return GetComponent<TagComponent>().Tag;
}

Entity::operator uint32_t() const
{
	return static_cast<uint32_t>(m_entity);
}

Entity::operator entt::entity() const
{
	return m_entity;
}


SAND_TABLE_NAMESPACE_END


