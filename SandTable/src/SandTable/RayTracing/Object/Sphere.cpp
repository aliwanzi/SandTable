#include "pch.h"
#include "Sphere.h"

SAND_TABLE_NAMESPACE_BEGIN

Sphere::Sphere(uint32_t uiEntitID):
	Object(uiEntitID),
	m_vec3Position(glm::dvec3(0.f)),
	m_fRadius(0)
{
}

void Sphere::SetPosition(const glm::dvec3& vec3Position)
{
	m_vec3Position = vec3Position;
	m_bDirty = true;
}

const glm::dvec3& Sphere::GetPosition() const
{
	return m_vec3Position;
}

void Sphere::SetRadius(double fRadius)
{
	m_fRadius = fRadius;
	m_bDirty = true;
}

float Sphere::GetRadius() const
{
	return m_fRadius;
}

bool Sphere::Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord)
{
	auto& origin = ray.Origin - m_vec3Position;

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
	hitRecord.SetWorldNormal(ray, (hitRecord.WorldPosition - m_vec3Position) / m_fRadius);
	CalculateSampleUV(hitRecord.WorldNormal, hitRecord.UV);
	return true;
}

bool Sphere::CreateBoundingBox(double dStepBegin, double dStepEnd)
{
	m_spBoundingBox = CreateRef<BoundingBox>();
	m_spBoundingBox->SetMin(m_vec3Position - glm::dvec3(m_fRadius));
	m_spBoundingBox->SetMax(m_vec3Position + glm::dvec3(m_fRadius));
	return true;
}

void Sphere::CalculateSampleUV(const glm::vec3& SamplePoint, glm::dvec2& UV) const
{
	UV = glm::dvec2(glm::atan(SamplePoint.z, SamplePoint.x), glm::asin(SamplePoint.y));
	UV *= (1 / (2 * glm::pi<double>()), 1 / glm::pi<double>());
	UV += glm::dvec2(0.5);
}

SAND_TABLE_NAMESPACE_END
