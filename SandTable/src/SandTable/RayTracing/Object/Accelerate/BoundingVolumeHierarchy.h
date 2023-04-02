#pragma once
#include "SandTable/RayTracing/Object/ObjectContainer.h"

SAND_TABLE_NAMESPACE_BEGIN

class BoundingVolumeHierarchy:public Object
{
public:
	virtual ~BoundingVolumeHierarchy() = default;
	BoundingVolumeHierarchy();
	BoundingVolumeHierarchy(Ref<ObjectContainer> spObjectContainer, uint32_t uiEntitID);
	// Í¨¹ý Hittable ¼Ì³Ð
	virtual bool Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord) const override;

	void SetMaterialID(uint32_t uiMaterialID) override;

private:
	void CreateBVHNode(std::vector<Ref<Object>>& vecObject);

	virtual bool CreateBoundingBox(double dStepBegin, double dStepEnd) override;
private:
	Ref<ObjectContainer> m_spObjectContainer;
	Ref<Object> m_spLeft;
	Ref<Object> m_spRight;
};

SAND_TABLE_NAMESPACE_END

