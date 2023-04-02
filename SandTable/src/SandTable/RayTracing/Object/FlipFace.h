#pragma once
#include "Object.h"
SAND_TABLE_NAMESPACE_BEGIN
class FlipFace:public Object
{
public:
	FlipFace(Ref<Object>spObject);
	virtual ~FlipFace() = default;
	

	// Í¨¹ý Object ¼Ì³Ð
	virtual bool Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord) const override;

	virtual bool CreateBoundingBox(double dStepBegin, double dStepEnd) override;

	virtual void SetMaterialID(uint32_t uiMaterialID);
private:
	Ref<Object> m_spObject;

};
SAND_TABLE_NAMESPACE_END

