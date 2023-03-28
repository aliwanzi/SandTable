#pragma once
#include "Material.h"

SAND_TABLE_NAMESPACE_BEGIN
class ColorTexture;
class Isotropic:public Material
{
public:
	Isotropic(Ref<ColorTexture> spColorTexture, uint32_t uiMateriaID);
	virtual ~Isotropic() = default;
	// Í¨¹ý Material ¼Ì³Ð
	virtual bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, glm::dvec3& attenuation, Ray& rayOut) const override;
private:
	Ref<ColorTexture> m_spColorTexture;
};

SAND_TABLE_NAMESPACE_END

