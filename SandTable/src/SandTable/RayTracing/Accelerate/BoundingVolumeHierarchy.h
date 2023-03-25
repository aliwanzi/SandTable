#pragma once
#include "SandTable/RayTracing/Object/ObjectContainer.h"
#include "SandTable/RayTracing/Accelerate/Accelerate.h"

SAND_TABLE_NAMESPACE_BEGIN

class BoundingVolumeHierarchy:public Accelerate
{
public:
	BoundingVolumeHierarchy(std::shared_ptr<ObjectContainer> spObjectContainer, double dStepBegin, double dStepEnd);
	BoundingVolumeHierarchy(std::vector<Ref<Object>>& vecObject, size_t start, size_t end, double dStepBegin, double dStepEnd);

	// Í¨¹ý Hittable ¼Ì³Ð
	virtual bool Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord) override;

private:
	virtual bool CreateBoundingBox(double dStepBegin, double dStepEnd) override;
private:
	Ref<Hittable> m_spLeft;
	Ref<Hittable> m_spRight;
};

SAND_TABLE_NAMESPACE_END

