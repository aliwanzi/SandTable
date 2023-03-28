#pragma once
#include "Box.h"

SAND_TABLE_NAMESPACE_BEGIN
class Isotropic;
class ColorTexture;
class ConstantMedium:public Object
{
public:
	ConstantMedium(Ref<Box> spBox,double dDensity);
	virtual ~ConstantMedium() = default;
	// ͨ�� Object �̳�
	virtual bool Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord) override;
	virtual bool CreateBoundingBox(double dStepBegin, double dStepEnd) override;
private:
	Ref<Box> m_spBox;
	Ref<Isotropic> m_spIsoMaterial;
	double m_dDensity;
};

SAND_TABLE_NAMESPACE_END

