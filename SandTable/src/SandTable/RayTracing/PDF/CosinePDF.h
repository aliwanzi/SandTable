#pragma once
#include "PDF.h"

SAND_TABLE_NAMESPACE_BEGIN
class CosinePDF:public PDF
{
public:
	CosinePDF();
	// ͨ�� PDF �̳�
	virtual double GetPDF(const glm::dvec3& direction) const override;
	virtual glm::dvec3 GeneratePDF() const override;
};
SAND_TABLE_NAMESPACE_END

