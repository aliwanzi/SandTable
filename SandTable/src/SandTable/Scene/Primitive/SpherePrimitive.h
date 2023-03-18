#pragma once
#include "SandTable/RayTracing/Material.h"

SAND_TABLE_NAMESPACE_BEGIN
class SpherePrimitive
{
public:

	SpherePrimitive(const glm::vec3& vec3Position, float fRadius, uint32_t uiEntitID);

	void SetPosition(const glm::vec3& vec3Position);
	glm::vec3& GetPosition();

	void SetRadius(float fRadius);
	float& GetRadius();

	void SetMaterial(const Ref<Material>& spMaterial);
	Ref<Material>& GetMaterial();

	uint32_t GetEntityID()const;
	void SetEntityID(uint32_t uiEntityID);

private:
	glm::vec3 m_vec3Position;
	uint32_t m_uiEntitID;
	float m_fRadius;
	Ref<Material> m_spMaterial;
};

using MapSphere = std::map<uint32_t, Ref<SpherePrimitive>>;

SAND_TABLE_NAMESPACE_END

