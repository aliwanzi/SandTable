#include "pch.h"
#include "CosinePDF.h"
#include "OrthoNormalBasis.h"

SAND_TABLE_NAMESPACE_BEGIN
CosinePDF::CosinePDF(const glm::dvec3& normal) :
	m_spOrthoNormalBasis(CreateRef<OrthoNormalBasis>(normal))
{

}

double CosinePDF::GetPDF(const glm::dvec3& direction) const
{
	auto cosine = glm::dot(m_spOrthoNormalBasis->GetW(), glm::normalize(direction));
	return cosine < 0 ? 0 : cosine / glm::pi<double>();
}

glm::dvec3 CosinePDF::SampleDirection() const
{
	return m_spOrthoNormalBasis->Local(Random::CosineDirection());
}

SAND_TABLE_NAMESPACE_END