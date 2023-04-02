#pragma once
#include "SandTable/RayTracing/Object/Object.h"
SAND_TABLE_NAMESPACE_BEGIN
class XYRectangle:public Object
{
public:
	XYRectangle(uint32_t uiEntitID);
	~XYRectangle() = default;
	void SetXYPoints(const glm::dvec2& PointXY0, const glm::dvec2& PointXY1);
	void SetZPos(double dZPos);
	// Í¨¹ý Object ¼Ì³Ð
	virtual bool Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord) const override;
	virtual bool CreateBoundingBox(double dStepBegin, double dStepEnd) override;
private:
	glm::dvec2 m_vec2PointMin;
	glm::dvec2 m_vec2PointMax;
	double m_dZPos;
};
SAND_TABLE_NAMESPACE_END

