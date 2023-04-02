#pragma once
#include "SandTable/RayTracing/Material/Material.h"
#include "SandTable/RayTracing/Color/ColorTexture.h"
SAND_TABLE_NAMESPACE_BEGIN

class DiffuseLight :public Material
{
public:
	DiffuseLight(Ref<ColorTexture> spColorTexture, uint32_t uiMaterialID);
	~DiffuseLight() = default;
	// Í¨¹ý Material ¼Ì³Ð
	virtual bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, ScatterRecord& scatterRecord) const override;
	virtual glm::dvec3 Emitted(const HitRecord& hitRecord)const;
private:
	Ref<ColorTexture> m_spColorTexture;
};
SAND_TABLE_NAMESPACE_END

