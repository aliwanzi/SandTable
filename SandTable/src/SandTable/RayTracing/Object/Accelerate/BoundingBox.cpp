#include "pch.h"
#include "BoundingBox.h"

SAND_TABLE_NAMESPACE_BEGIN

BoundingBox::BoundingBox():
	m_vec3Max(-Random::FloatMax()),
	m_vec3Min(Random::FloatMax())
{

}

BoundingBox::BoundingBox(const glm::dvec3 min, const glm::dvec3 max)
{
	m_vec3Max = glm::max(min, max);
	m_vec3Min = glm::min(min, max);
}

void BoundingBox::SetMin(const glm::dvec3& vec3Min)
{
	m_vec3Min = vec3Min;
}

const glm::dvec3& BoundingBox::GetMin() const
{
	return m_vec3Min;
}

void BoundingBox::SetMax(const glm::dvec3& vec3Max)
{
	m_vec3Max = vec3Max;
}

const glm::dvec3& BoundingBox::GetMax() const
{
	return m_vec3Max;
}

bool BoundingBox::Intersect(const Ray& ray, double& stepMin, double& stepMax) const
{

	glm::dvec3 min{ (m_vec3Min - ray.Origin) * (1.0 / ray.Direction) };
	glm::dvec3 max{ (m_vec3Max - ray.Origin) * (1.0 / ray.Direction) };

	stepMin = glm::max(glm::compMax(glm::min(min, max)), stepMin);
	stepMax = glm::min(glm::compMin(glm::max(min, max)), stepMax);

	return stepMax > stepMin;
}

bool BoundingBox::Contains(const glm::dvec3& point) const
{
	return point.x >= m_vec3Min.x && point.y >= m_vec3Min.y && point.z >= m_vec3Min.z &&
		point.x <= m_vec3Max.x && point.y <= m_vec3Max.y && point.z <= m_vec3Max.z;
}

glm::dvec3 BoundingBox::GetDimension() const
{
	return m_vec3Max - m_vec3Min;
}

glm::dvec3 BoundingBox::GetCentroid() const
{
	auto center = (m_vec3Max + m_vec3Min) / 2.0;
	return center;
}

double BoundingBox::GetArea() const
{
	if (!Valid()) 
		return 0.0;
	auto dim = m_vec3Max - m_vec3Min;
	return 2.0 * (dim.x * dim.y + dim.x * dim.z + dim.y * dim.z);
}

int BoundingBox::GetMaxExtent() const
{
	glm::dvec3 dim = m_vec3Max - m_vec3Min;
	if (dim.x > dim.y && dim.x > dim.z)
		return 0;
	else if (dim.y > dim.z)
		return 1;
	else
		return 2;
	return 0;
}

void BoundingBox::Merge(Ref<BoundingBox> spBoundingBox)
{
	m_vec3Min = glm::min(m_vec3Min, spBoundingBox->GetMin());
	m_vec3Max = glm::max(m_vec3Max, spBoundingBox->GetMax());
}

void BoundingBox::SurrondingBox(Ref<BoundingBox> spLeftBoundingBox, Ref<BoundingBox> spRightBoundingBox)
{
	m_vec3Min = glm::min(spLeftBoundingBox->GetMin(), spRightBoundingBox->GetMin());
	m_vec3Max = glm::max(spLeftBoundingBox->GetMax(), spRightBoundingBox->GetMax());
}

void BoundingBox::Merge(const glm::dvec3& p)
{
	m_vec3Min = glm::min(m_vec3Min, p);
	m_vec3Max = glm::max(m_vec3Max, p);
}

bool BoundingBox::Valid() const
{
	return m_vec3Min.x > m_vec3Max.x ||
		m_vec3Min.y > m_vec3Max.y ||
		m_vec3Min.z > m_vec3Max.z;
}

SAND_TABLE_NAMESPACE_END
