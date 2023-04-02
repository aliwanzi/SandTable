#pragma once
SAND_TABLE_NAMESPACE_BEGIN

class PDF
{
public:
	virtual ~PDF() = default;
	virtual double GetPDF(const glm::dvec3& direction)const = 0;
	virtual glm::dvec3 SampleDirection()const = 0;
};

SAND_TABLE_NAMESPACE_END

