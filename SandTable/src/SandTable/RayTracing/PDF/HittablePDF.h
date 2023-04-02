#pragma once
#include "PDF.h"
SAND_TABLE_NAMESPACE_BEGIN
class Object;
class HittablePDF:public PDF
{
public:
	virtual ~HittablePDF() = default;
	HittablePDF(Ref<Object> spSampleObject,const glm::dvec3& hitPoint);
	// Í¨¹ý PDF ¼Ì³Ð
	virtual double GetPDF(const glm::dvec3& direction) const override;
	virtual glm::dvec3 SampleDirection() const override;
protected:
	glm::dvec3 m_vec3HitPoint;
	Ref<Object> m_spSampleObject;
};

SAND_TABLE_NAMESPACE_END

