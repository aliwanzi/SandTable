#pragma once
#include "Material.h"

SAND_TABLE_NAMESPACE_BEGIN

class Dielectric :public Material
{
public:
	Dielectric(uint32_t uiMaterialID);
	// Í¨¹ý Material ¼Ì³Ð
	virtual bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, glm::dvec3& attenuation, Ray& rayOut) const override;
	void SetMetallic(float fMetallic);
	float GetMetallic() const;

private:
	float Reflectance(float fCosTheta, float fRatio) const;

private:
	float m_fMetallic;
};

SAND_TABLE_NAMESPACE_END

