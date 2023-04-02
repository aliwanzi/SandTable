#include "pch.h"
#include "CosinePDF.h"

SAND_TABLE_NAMESPACE_BEGIN
double CosinePDF::GetPDF(const glm::dvec3& direction) const
{
    return 0.0;
}

glm::dvec3 CosinePDF::GeneratePDF() const
{
    return glm::dvec3();
}

SAND_TABLE_NAMESPACE_END