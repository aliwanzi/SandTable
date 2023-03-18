#include "pch.h"
#include "SpherePrimitive.h"

SAND_TABLE_NAMESPACE_BEGIN

SpherePrimitive::SpherePrimitive(const glm::vec3& vec3Position, float fRadius, Ref<RayTracingMaterial> spRayTracingMaterial):
	m_vec3Position(vec3Position),
	m_fRadius(fRadius),
	m_spMaterial(spRayTracingMaterial)
{
}

glm::vec3& SpherePrimitive::GetPosition()
{
	return m_vec3Position;
}

float& SpherePrimitive::GetRadius()
{
	return m_fRadius;
}

Ref<RayTracingMaterial>& SpherePrimitive::GetMaterial()
{
	return m_spMaterial;
}

SAND_TABLE_NAMESPACE_END
