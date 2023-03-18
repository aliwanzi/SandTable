#pragma once

SAND_TABLE_NAMESPACE_BEGIN
class RayTracingMaterial
{
public:
	RayTracingMaterial(const glm::vec3& vec3Albedo);

	glm::vec3& GetAlbedo();
private:
	glm::vec3 m_vec3Albedo;
};
SAND_TABLE_NAMESPACE_END

