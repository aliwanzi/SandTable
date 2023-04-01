#pragma once
#include "Box.h"

SAND_TABLE_NAMESPACE_BEGIN
class Isotropic;
class ColorTexture;
class ConstantMedium:public Object
{
public:
	ConstantMedium(Ref<Object> spObject,double dDensity);
	virtual ~ConstantMedium() = default;
	// Í¨¹ý Object ¼Ì³Ð
	virtual bool Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord) override;
	virtual bool CreateBoundingBox(double dStepBegin, double dStepEnd) override;
private:
	Ref<Object> m_spObject;
	Ref<Isotropic> m_spIsoMaterial;
	double m_dDensity;
};

SAND_TABLE_NAMESPACE_END

