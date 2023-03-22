#pragma once
#include "Material.h"

SAND_TABLE_NAMESPACE_BEGIN
class Metal: public Material
{
public:
	Metal(uint32_t uiMaterialID);

	// Í¨¹ý Material ¼Ì³Ð
	virtual bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, glm::vec3& attenuation, Ray& rayOut) const override;

	void SetAlbedo(const glm::vec3& vec3Albedo);
	const glm::vec3& GetAlbedo() const;

	void SetRoughness(float fRoughness);
	float GetRoughness() const;

private:
	float m_fRoughness;
	glm::vec3 m_vec3Albedo;
};
SAND_TABLE_NAMESPACE_END

