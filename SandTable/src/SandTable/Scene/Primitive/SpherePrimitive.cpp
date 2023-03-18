#include "pch.h"
#include "SpherePrimitive.h"

SAND_TABLE_NAMESPACE_BEGIN
SpherePrimitive::SpherePrimitive(const glm::vec3& vec3Position, float fRadius, uint32_t uiEntitID):
	m_vec3Position(vec3Position),
	m_fRadius(fRadius),
	m_uiEntitID(uiEntitID),
	m_spMaterial(nullptr)
{
}

void SpherePrimitive::SetPosition(const glm::vec3& vec3Position)
{
	m_vec3Position = vec3Position;
}

glm::vec3& SpherePrimitive::GetPosition()
{
	return m_vec3Position;
}

void SpherePrimitive::SetRadius(float fRadius)
{
	m_fRadius = fRadius;
}

float& SpherePrimitive::GetRadius()
{
	return m_fRadius;
}

void SpherePrimitive::SetMaterial(const Ref<Material>& spMaterial)
{
	m_spMaterial = spMaterial;
}

Ref<Material>& SpherePrimitive::GetMaterial()
{
	return m_spMaterial;
}

uint32_t SpherePrimitive::GetEntityID() const
{
	return m_uiEntitID;
}

void SpherePrimitive::SetEntityID(uint32_t uiEntityID)
{
	m_uiEntitID = uiEntityID;
}

SAND_TABLE_NAMESPACE_END
