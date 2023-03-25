#pragma once
#include "SandTable/RayTracing/Object/Object.h"

SAND_TABLE_NAMESPACE_BEGIN
class ObjectContainer :public Hittable
{
public:
	ObjectContainer();
	~ObjectContainer() = default;
	// Í¨¹ý Hittable ¼Ì³Ð
	virtual bool Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord) override;
	virtual bool CreateBoundingBox(double dStepBegin, double dStepEnd) override;
	virtual const Ref<BoundingBox>& GetBoundingBox()const override;
	
	std::vector<Ref<Object>>& GetAllObject();

	void AddObject(std::shared_ptr<Object> spObject);
	void Clear();
private:
	std::vector<Ref<Object>> m_vecObject;
	Ref<BoundingBox> m_spBoundingBox;
	bool m_bInitilize;
};

SAND_TABLE_NAMESPACE_END

