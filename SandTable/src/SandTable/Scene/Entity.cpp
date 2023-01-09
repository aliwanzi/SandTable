#include "pch.h"
#include "Entity.h"

SAND_TABLE_NAMESPACE_BEGIN

Entity::Entity(const Ref<entt::registry>& spRegistry) :
	m_spRegistry(spRegistry)
{
	m_spEntity = CreateRef<entt::entity>(m_spRegistry->create());
}

Entity::Entity()
{
	m_spRegistry = CreateRef<entt::registry>();
	m_spEntity = CreateRef<entt::entity>(m_spRegistry->create());
}

Entity::Entity(const Ref<entt::registry>& spRegistry, const Ref<entt::entity>& spEntity):
	m_spRegistry(spRegistry),
	m_spEntity(spEntity)
{

}

//Entity& Entity::operator=(const Entity& entity)
//{
//	m_spEntity = entity.m_spEntity;
//	m_spRegistry = entity.m_spRegistry;
//	return *this;
//}
//
bool Entity::operator==(const Ref<Entity>& spEntity) const
{
	return m_spEntity == spEntity->m_spEntity;
}

bool Entity::operator!=(const Ref<Entity>& spEntity) const
{
	return !(*this == spEntity);
}

Entity::operator uint32_t() const
{
	return static_cast<uint32_t>(*m_spEntity);
}

Entity::operator entt::entity() const
{
	return *m_spEntity;
}


SAND_TABLE_NAMESPACE_END


