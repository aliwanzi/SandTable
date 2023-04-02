#pragma once
#include "SandTable/RayTracing/Object/Object.h"

SAND_TABLE_NAMESPACE_BEGIN
class XZRectangle :public Object
{
public:
	XZRectangle(uint32_t uiEntitID);
	~XZRectangle() = default;
	void SetXZPoints(const glm::dvec2& PointXZ0, const glm::dvec2& PointXZ1);
	void SetYPos(double dYPos);

	// Í¨¹ý Object ¼Ì³Ð
	virtual bool Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord) const override;
	virtual bool CreateBoundingBox(double dStepBegin, double dStepEnd) override;

	virtual glm::dvec3 SampleDirection(const glm::dvec3& vec3HitPoint) const;
	virtual double GetPDF(const glm::dvec3& vec3HitPoint, const glm::dvec3& direction)const;
private:
	glm::dvec2 m_vec2PointMin;
	glm::dvec2 m_vec2PointMax;
	double m_dYPos;
};
SAND_TABLE_NAMESPACE_END

