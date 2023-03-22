#pragma once
#include "Material.h"

SAND_TABLE_NAMESPACE_BEGIN
class Metal: public Material
{
public:
	Metal(uint32_t uiMaterialID);

	// Í¨¹ý Material ¼Ì³Ð
	virtual bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, glm::dvec3& attenuation, Ray& rayOut) const override;

	void SetAlbedo(const glm::dvec3& vec3Albedo);
	const glm::dvec3& GetAlbedo() const;

	void SetRoughness(double fRoughness);
	float GetRoughness() const;

private:
	double m_fRoughness;
	glm::dvec3 m_vec3Albedo;
};
SAND_TABLE_NAMESPACE_END

