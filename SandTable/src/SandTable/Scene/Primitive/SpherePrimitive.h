#pragma once
#include "SandTable/RayTracing/RayTracingMaterial.h"

SAND_TABLE_NAMESPACE_BEGIN
class SpherePrimitive
{
public:
	SpherePrimitive(const glm::vec3& vec3Position, float Radius, Ref<RayTracingMaterial> spRayTracingMaterial);
	glm::vec3& GetPosition();
	float& GetRadius();
	Ref<RayTracingMaterial>& GetMaterial();
private:
	glm::vec3 m_vec3Position;
	float m_fRadius;
	Ref<RayTracingMaterial> m_spMaterial;
};

SAND_TABLE_NAMESPACE_END

