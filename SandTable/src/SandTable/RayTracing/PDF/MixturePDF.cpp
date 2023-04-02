#include "pch.h"
#include "MixturePDF.h"
SAND_TABLE_NAMESPACE_BEGIN
MixturePDF::MixturePDF(Ref<PDF> spPDF0, Ref<PDF> spPDF1):
    m_spPDF0(spPDF0),m_spPDF1(spPDF1)
{
}

double MixturePDF::GetPDF(const glm::dvec3& direction) const
{
    return 0.5 * m_spPDF0->GetPDF(direction) + 0.5 * m_spPDF1->GetPDF(direction);
}

glm::dvec3 MixturePDF::SampleDirection() const
{
    if (Random::Float()<0.5)
    {
        return m_spPDF0->SampleDirection();
    }
    else
    {
        return m_spPDF1->SampleDirection();
    }
}
SAND_TABLE_NAMESPACE_END