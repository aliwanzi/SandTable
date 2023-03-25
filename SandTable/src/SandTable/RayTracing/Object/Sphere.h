#pragma once
#include "Object.h"

SAND_TABLE_NAMESPACE_BEGIN
class Sphere :public Object
{
public:
	Sphere(uint32_t uiEntitID);
	virtual ~Sphere() = default;

	void SetPosition(const glm::dvec3& vec3Position);
	const glm::dvec3& GetPosition() const;

	void SetRadius(double fRadius);
	float GetRadius() const;

	// Í¨¹ý Hittable ¼Ì³Ð
	virtual bool Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord) override;

	virtual bool CreateBoundingBox(double dStepBegin, double dStepEnd);
private:
	void CalculateSampleUV(const glm::vec3& SamplePoint, glm::dvec2& UV) const;

protected:
	glm::dvec3 m_vec3Position;
	double m_fRadius;
};

SAND_TABLE_NAMESPACE_END

