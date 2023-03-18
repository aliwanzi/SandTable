#include "pch.h"
#include "RayTracingMaterial.h"
SAND_TABLE_NAMESPACE_BEGIN
RayTracingMaterial::RayTracingMaterial(const glm::vec3& vec3Albedo):
	m_vec3Albedo(vec3Albedo)
{
}

glm::vec3& RayTracingMaterial::GetAlbedo()
{
	return m_vec3Albedo;
}

SAND_TABLE_NAMESPACE_END
