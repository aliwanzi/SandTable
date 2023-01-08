#include "pch.h"
#include "ScriptableEntity.h"

SAND_TABLE_NAMESPACE_BEGIN
ScriptableEntity::ScriptableEntity():m_spEntity(nullptr)
{

}

void ScriptableEntity::SetEntity(const Ref<Entity> spEntity)
{
	m_spEntity = spEntity;
}

SAND_TABLE_NAMESPACE_END
