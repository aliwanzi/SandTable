#include "pch.h"
#include "HittablePDF.h"
#include "SandTable/RayTracing/Object/Object.h"

SAND_TABLE_NAMESPACE_BEGIN

HittablePDF::HittablePDF(Ref<Object> spObject, const glm::dvec3& origin) :
	m_spSampleObject(spObject), m_vec3HitPoint(origin)
{
}

double HittablePDF::GetPDF(const glm::dvec3& direction) const
{
	return m_spSampleObject->GetPDF(m_vec3HitPoint, direction);
}

glm::dvec3 HittablePDF::SampleDirection() const
{
	return m_spSampleObject->SampleDirection(m_vec3HitPoint);
}

SAND_TABLE_NAMESPACE_END