#pragma once
#include "PDF.h"
SAND_TABLE_NAMESPACE_BEGIN
class MixturePDF:PDF
{
public:
	MixturePDF(Ref<PDF> spPDF0, Ref<PDF> spPDF1);
	// Í¨¹ý PDF ¼Ì³Ð
	virtual double GetPDF(const glm::dvec3& direction) const override;
	virtual glm::dvec3 SampleDirection() const override;
private:
	Ref<PDF> m_spPDF0;
	Ref<PDF> m_spPDF1;
};
SAND_TABLE_NAMESPACE_END

