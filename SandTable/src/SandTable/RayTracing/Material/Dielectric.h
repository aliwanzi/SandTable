#pragma once
#include "Material.h"

SAND_TABLE_NAMESPACE_BEGIN

class Dielectric :public Material
{
public:
	Dielectric(uint32_t uiMaterialID);
	virtual ~Dielectric() = default;
	// ͨ�� Material �̳�
	virtual bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, ScatterRecord& scatterRecord) const override;
	void SetMetallic(float fMetallic);
	float GetMetallic() const;

private:
	float Reflectance(float fCosTheta, float fRatio) const;

private:
	float m_fMetallic;
};

SAND_TABLE_NAMESPACE_END

