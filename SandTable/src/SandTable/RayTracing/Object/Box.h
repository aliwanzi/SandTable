#pragma once
#include "Object.h"
#include "ObjectContainer.h"
SAND_TABLE_NAMESPACE_BEGIN

class Box :public Object
{
public:
	Box(const glm::dvec3& PointMin, const glm::dvec3& PointMax, uint32_t uiEntityID);
	virtual ~Box() = default;

	// Í¨¹ý Object ¼Ì³Ð
	virtual bool Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord) const override;
	virtual bool CreateBoundingBox(double dStepBegin, double dStepEnd) override;
	virtual void SetMaterialID(uint32_t uiMaterialID) override;
private:
	Ref<ObjectContainer> m_spObjectContainer;
};
SAND_TABLE_NAMESPACE_END
