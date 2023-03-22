#pragma once
#include "SandTable/RayTracing/Object/Object.h"

SAND_TABLE_NAMESPACE_BEGIN
class ObjectContainer :public Hittable
{
public:
	// Í¨¹ý Hittable ¼Ì³Ð
	virtual bool Hit(const Ray& ray, float fMin, float fMax, HitRecord& hitRecord) const override;
	
	std::vector<Ref<Object>>& GetAllObject();

	void AddObject(std::shared_ptr<Object> spObject);
	void Clear();
private:
	std::vector<Ref<Object>> m_vecObject;
};

SAND_TABLE_NAMESPACE_END

