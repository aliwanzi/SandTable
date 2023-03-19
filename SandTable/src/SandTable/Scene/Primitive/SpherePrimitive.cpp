#include "pch.h"
#include "SpherePrimitive.h"

SAND_TABLE_NAMESPACE_BEGIN
SpherePrimitive::SpherePrimitive(const glm::vec3& vec3Position, float fRadius, uint32_t uiEntitID):
	m_vec3Position(vec3Position),
	m_fRadius(fRadius),
	m_uiEntitID(uiEntitID),
	m_uiMaterialID(-1),
	m_bDirty(false)
{
}

void SpherePrimitive::SetPosition(const glm::vec3& vec3Position)
{
	m_vec3Position = vec3Position;
	m_bDirty = true;
}

const glm::vec3& SpherePrimitive::GetPosition() const
{
	return m_vec3Position;
}

void SpherePrimitive::SetRadius(float fRadius) 
{
	m_fRadius = fRadius;
	m_bDirty = true;
}

float SpherePrimitive::GetRadius() const
{
	return m_fRadius;
}

void SpherePrimitive::SetMaterialID(uint32_t uiMaterialID)
{
	m_uiMaterialID = uiMaterialID;
	m_bDirty = true;
}

uint32_t SpherePrimitive::GetMaterialID() const
{
	return m_uiMaterialID;
}

void SpherePrimitive::SetEntityID(uint32_t uiEntityID)
{
	m_uiEntitID = uiEntityID;
	m_bDirty = true;
}

uint32_t SpherePrimitive::GetEntityID() const
{
	return m_uiEntitID;
}

void SpherePrimitive::ResetDirty()
{
	m_bDirty = false;
}

bool SpherePrimitive::GetDirty()
{
	return m_bDirty;
}

SAND_TABLE_NAMESPACE_END
