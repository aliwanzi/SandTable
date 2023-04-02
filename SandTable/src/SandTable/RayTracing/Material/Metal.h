#pragma once
#include "Material.h"

SAND_TABLE_NAMESPACE_BEGIN
class Metal : public Material
{
public:
	Metal(uint32_t uiMaterialID);
	virtual ~Metal() = default;

	// ͨ�� Material �̳�
	virtual bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, ScatterRecord& scatterRecord) const override;

	void SetAlbedo(const glm::dvec3& vec3Albedo);
	const glm::dvec3& GetAlbedo() const;

	void SetRoughness(double fRoughness);
	float GetRoughness() const;

private:
	double m_fRoughness;
	glm::dvec3 m_vec3Albedo;
};
SAND_TABLE_NAMESPACE_END

