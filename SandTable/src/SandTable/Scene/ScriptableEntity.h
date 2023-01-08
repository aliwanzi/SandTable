#pragma once
#include "SandTable/Scene/Entity.h"

SAND_TABLE_NAMESPACE_BEGIN
class ScriptableEntity
{
public:
	ScriptableEntity();
	virtual ~ScriptableEntity() = default;
	void SetEntity(const Ref<Entity> spEntity);

	template<typename T>
	T& GetComponent()
	{
		return m_spEntity->GetComponent<T>();
	}
	virtual void OnCreate() {}
	virtual void OnUpdate(TimeStep ts) {}
private:
	Ref<Entity> m_spEntity;
};

SAND_TABLE_NAMESPACE_END