#include "pch.h"
#include "OrthoNormalBasis.h"
SAND_TABLE_NAMESPACE_BEGIN

OrthoNormalBasis::OrthoNormalBasis(const glm::dvec3& normal)
{
	m_W = glm::normalize(normal);
	glm::dvec3 dir = (glm::abs(normal.x) > 0.9) ? glm::dvec3(0, 1, 0) : glm::dvec3(1, 0, 0);
	m_V = glm::normalize(glm::cross(m_W, dir));
	m_U = glm::cross(m_W, m_V);
}

glm::dvec3 OrthoNormalBasis::Local(const glm::dvec3& vector) const
{
	return vector.x * m_U + vector.y * m_V + vector.z * m_W;
}

const glm::dvec3& OrthoNormalBasis::GetU() const
{
	return m_U;
}

const glm::dvec3& OrthoNormalBasis::GetV() const
{
	return m_V;
}

const glm::dvec3& OrthoNormalBasis::GetW() const
{
	return m_W;
}


SAND_TABLE_NAMESPACE_END
