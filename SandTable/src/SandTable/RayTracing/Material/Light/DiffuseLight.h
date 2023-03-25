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
	virtual bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, glm::dvec3& attenuation, Ray& rayOut) const override;
	virtual glm::dvec3 Emitted(const glm::dvec3& SamplePoint, const glm::dvec2& UV)const;
private:
	Ref<ColorTexture> m_spColorTexture;
};
SAND_TABLE_NAMESPACE_END

