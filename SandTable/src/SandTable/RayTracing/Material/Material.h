#pragma once
#include "SandTable/RayTracing/Ray.h"
#include "SandTable/RayTracing/Hittable.h"

SAND_TABLE_NAMESPACE_BEGIN

class Material
{
public:
	Material(uint32_t uiMaterialID);
	virtual bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, glm::dvec3& attenuation, Ray& rayOut) const = 0;

	uint32_t GetMaterialID() const;

	void ResetDirty();
	bool GetDirty();

protected:
	bool m_bDirty;

private:
	uint32_t m_uiMaterialID;
};

using MapMaterial = std::map<uint32_t, Ref<Material>>;
SAND_TABLE_NAMESPACE_END

