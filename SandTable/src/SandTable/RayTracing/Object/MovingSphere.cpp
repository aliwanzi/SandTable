#include "pch.h"
#include "MovingSphere.h"
SAND_TABLE_NAMESPACE_BEGIN

MovingSphere::MovingSphere(uint32_t uiEntitID) :Sphere(uiEntitID)
{
}

void MovingSphere::SetStepBegin(double fStepBegin)
{
	m_bDirty = true;
	m_fStepBegin = fStepBegin;
}

void MovingSphere::SetStepEnd(double fStepEnd)
{
	m_bDirty = true;
	m_fStepEnd = fStepEnd;
}

void MovingSphere::SetMovePosition(const glm::dvec3& vec3MovePosition)
{
	m_vec3MovePosition = vec3MovePosition;
}

const glm::dvec3& MovingSphere::GetMovePosition() const
{
	return m_vec3MovePosition;
}

bool MovingSphere::Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord) const
{
	auto& origin = ray.Origin - Center(ray.Step);

	float fA = glm::dot(ray.Direction, ray.Direction);
	float fB = 2.f * glm::dot(origin, ray.Direction);
	float fC = glm::dot(origin, origin) - m_fRadius * m_fRadius;

	// Find the nearest root that lies in the acceptable range.
	float discriminant = fB * fB - 4.0 * fA * fC;
	if (discriminant < 0)
	{
		return false;
	}

	auto root = (-fB - sqrt(discriminant)) / (2. * fA);
	if (root < fMin || fMax < root)
	{
		root = (-fB + sqrt(discriminant)) / (2. * fA);
		if (root < fMin || fMax < root)
			return false;
	}

	hitRecord.HitDistance = root;
	hitRecord.WorldPosition = ray.Origin + ray.Direction * hitRecord.HitDistance;
	hitRecord.EntityID = m_uiEntitID;
	hitRecord.MaterialID = m_uiMaterialID;
	hitRecord.SetWorldNormal(ray, (hitRecord.WorldPosition - Center(ray.Step)) / m_fRadius);
	return true;
}

bool MovingSphere::CreateBoundingBox(double dStepBegin, double dStepEnd)
{
	m_spBoundingBox->Merge(Center(dStepBegin) - glm::dvec3(m_fRadius));
	m_spBoundingBox->Merge(Center(dStepBegin) + glm::dvec3(m_fRadius));

	m_spBoundingBox->Merge(Center(dStepEnd) - glm::dvec3(m_fRadius));
	m_spBoundingBox->Merge(Center(dStepEnd) + glm::dvec3(m_fRadius));
	return true;
}

glm::dvec3 MovingSphere::Center(double fStep) const
{
	return m_vec3Position + (fStep - m_fStepBegin) / (m_fStepEnd - m_fStepBegin) * (m_vec3MovePosition - m_vec3Position);
}


SAND_TABLE_NAMESPACE_END
