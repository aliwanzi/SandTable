#pragma once
#include "SandTable/RayTracing/Object/Object.h"

SAND_TABLE_NAMESPACE_BEGIN
class ObjectContainer :public Object
{
public:
	ObjectContainer();
	~ObjectContainer() = default;
	// Í¨¹ý Hittable ¼Ì³Ð
	virtual bool Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord) const override;
	virtual bool CreateBoundingBox(double dStepBegin, double dStepEnd) override;

	virtual glm::dvec3 SampleDirection(const glm::dvec3& vec3HitPoint) const override;
	virtual double GetPDF(const glm::dvec3& vec3HitPoint, const glm::dvec3& direction)const override;

	std::vector<Ref<Object>>& GetAllObject();
	void AddObject(Ref<Object> spObject);
	void Clear();
private:
	std::vector<Ref<Object>> m_vecObject;
};

SAND_TABLE_NAMESPACE_END

