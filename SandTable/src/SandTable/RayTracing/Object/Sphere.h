#pragma once
#include "Object.h"

SAND_TABLE_NAMESPACE_BEGIN
class OrthoNormalBasis;
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
	virtual bool Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord) const override;

	virtual bool CreateBoundingBox(double dStepBegin, double dStepEnd);

	virtual glm::dvec3 SampleDirection(const glm::dvec3& vec3HitPoint) const override;
	virtual double GetPDF(const glm::dvec3& vec3HitPoint, const glm::dvec3& direction)const override;

private:
	void CalculateSampleUV(const glm::vec3& SamplePoint, glm::dvec2& UV) const;

protected:
	glm::dvec3 m_vec3Position;
	double m_fRadius;
};

SAND_TABLE_NAMESPACE_END

