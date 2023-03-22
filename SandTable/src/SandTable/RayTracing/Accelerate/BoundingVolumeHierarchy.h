#pragma once
#include "SandTable/RayTracing/Hittable.h"
#include "SandTable/RayTracing/Object/ObjectContainer.h"

SAND_TABLE_NAMESPACE_BEGIN

class BoundingVolumeHierarchy:public Hittable
{
public:
	BoundingVolumeHierarchy(std::shared_ptr<ObjectContainer> spObjectContainer, double dStepBegin, double dStepEnd);
	BoundingVolumeHierarchy(std::vector<Ref<Object>>& vecObject, size_t start, size_t end, double dStepBegin, double dStepEnd);

	// Í¨¹ý Hittable ¼Ì³Ð
	virtual bool Hit(const Ray& ray, float fMin, float fMax, HitRecord& hitRecord) const override;

private:
	virtual bool CreateBoundingBox(double dStepBegin, double dStepEnd) override;
	bool Comparator(const Ref<Hittable>& left, const Ref<Hittable>& right);
private:
	Ref<Hittable> m_spLeft;
	Ref<Hittable> m_spRight;
};

SAND_TABLE_NAMESPACE_END

