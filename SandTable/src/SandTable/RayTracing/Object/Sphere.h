#pragma once
#include "Object.h"

SAND_TABLE_NAMESPACE_BEGIN
class Sphere :public Object
{
public:
	Sphere(uint32_t uiEntitID);

	void SetPosition(const glm::dvec3& vec3Position);
	const glm::dvec3& GetPosition() const;

	void SetRadius(double fRadius);
	float GetRadius() const;

	// Í¨¹ý Hittable ¼Ì³Ð
	virtual bool Hit(const Ray& ray, float fMin, float fMax, HitRecord& hitRecord) const override;

	virtual bool CreateBoundingBox(double dStepBegin, double dStepEnd) { return true; };

protected:
	glm::dvec3 m_vec3Position;
	double m_fRadius;
};

SAND_TABLE_NAMESPACE_END

