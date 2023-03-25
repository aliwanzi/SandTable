#pragma once
#include "SandTable/RayTracing/Object/Object.h"

SAND_TABLE_NAMESPACE_BEGIN
class YZRectangle :public Object
{
public:
	YZRectangle(uint32_t uiEntitID);
	~YZRectangle() = default;
	void SetYZPoints(const glm::dvec2& PointYZ0, const glm::dvec2& PointYZ1);
	void SetXPos(double dXPos);

	// Í¨¹ý Object ¼Ì³Ð
	virtual bool Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord) override;
	virtual bool CreateBoundingBox(double dStepBegin, double dStepEnd) override;
private:
	glm::dvec2 m_vec2PointMin;
	glm::dvec2 m_vec2PointMax;
	double m_dXPos;
};
SAND_TABLE_NAMESPACE_END

