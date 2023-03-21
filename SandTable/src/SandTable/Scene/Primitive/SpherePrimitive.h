#pragma once
#include "SandTable/RayTracing/Ray.h"

SAND_TABLE_NAMESPACE_BEGIN
class SpherePrimitive
{
public:

	SpherePrimitive(const glm::vec3& vec3Position, float fRadius, uint32_t uiEntitID);

	void SetPosition(const glm::vec3& vec3Position);
	const glm::vec3& GetPosition() const;

	void SetRadius(float fRadius);
	float GetRadius() const;

	void SetMaterialID(uint32_t uiMaterialID);
	uint32_t GetMaterialID() const;

	void SetEntityID(uint32_t uiEntityID);
	uint32_t GetEntityID()const;

	void ResetDirty();
	bool GetDirty();

	bool Hit(const Ray& ray, HitPayLoad& hitPayLoad);

private:
	glm::vec3 m_vec3Position;
	float m_fRadius;
	uint32_t m_uiEntitID;
	uint32_t m_uiMaterialID;

	bool m_bDirty;
};

using MapSphere = std::map<uint32_t, Ref<SpherePrimitive>>;

SAND_TABLE_NAMESPACE_END

