#pragma once
#include "PDF.h"

SAND_TABLE_NAMESPACE_BEGIN
class OrthoNormalBasis;

class CosinePDF :public PDF
{
public:
	virtual ~CosinePDF() = default;
	CosinePDF(const glm::dvec3& normal);
	// ͨ�� PDF �̳�
	virtual double GetPDF(const glm::dvec3& direction) const override;
	virtual glm::dvec3 SampleDirection() const override;
private:
	Ref<OrthoNormalBasis> m_spOrthoNormalBasis;
};
SAND_TABLE_NAMESPACE_END

