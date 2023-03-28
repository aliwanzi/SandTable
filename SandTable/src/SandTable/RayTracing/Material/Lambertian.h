#pragma once
#include "Material.h"
SAND_TABLE_NAMESPACE_BEGIN

class ColorTexture;
class Lambertian :public Material
{
public:
	Lambertian(uint32_t uiMaterialID);
	virtual ~Lambertian() = default;

	// ͨ�� Material �̳�
	virtual bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, glm::dvec3& attenuation, Ray& rayOut) const override;

	void SetAlbedo(const glm::dvec3& vec3Albedo);
	const glm::dvec3& GetAlbedo() const;

	void SetColorTexture(Ref<ColorTexture> spColorTexture);
private:
	Ref<ColorTexture> m_spColorTexture;
	glm::dvec3 m_vec3Albedo;
};

SAND_TABLE_NAMESPACE_END

