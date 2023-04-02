#pragma once
#include "SandTable/RayTracing/Ray.h"
#include "SandTable/RayTracing/Hittable.h"

SAND_TABLE_NAMESPACE_BEGIN
struct ScatterRecord
{
	Ray SpecularRay;
	Ref<PDF> PDF = nullptr;
	bool IsSpecular;
	glm::dvec3 Attenuation;
};

class Material
{
public:
	Material(uint32_t uiMaterialID);
	virtual bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, ScatterRecord& scatterRecord) const;
	virtual double ScatterPDF(const Ray& rayIn, const HitRecord& hitRecord, Ray& rayOut) const;
	virtual glm::dvec3 Emitted(const HitRecord& hitRecord)const;

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

