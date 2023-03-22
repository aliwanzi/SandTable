#include "pch.h"
#include "Sphere.h"

SAND_TABLE_NAMESPACE_BEGIN
namespace
{
	const float fHitDistance = std::numeric_limits<float>::max();
}
Sphere::Sphere(uint32_t uiEntitID):
	Object(uiEntitID),
	m_vec3Position(glm::vec3(0.f)),
	m_fRadius(0)
{
}

void Sphere::SetPosition(const glm::vec3& vec3Position)
{
	m_vec3Position = vec3Position;
	m_bDirty = true;
}

const glm::vec3& Sphere::GetPosition() const
{
	return m_vec3Position;
}

void Sphere::SetRadius(float fRadius)
{
	m_fRadius = fRadius;
	m_bDirty = true;
}

float Sphere::GetRadius() const
{
	return m_fRadius;
}

bool Sphere::Hit(const Ray& ray, float fMin, float fMax, HitRecord& hitRecord) const
{
	auto& origin = ray.Origin - m_vec3Position;

	float fA = glm::dot(ray.Direction, ray.Direction);
	float fB = 2.f * glm::dot(origin, ray.Direction);
	float fC = glm::dot(origin, origin) - m_fRadius * m_fRadius;

	// Find the nearest root that lies in the acceptable range.
	float discriminant = fB * fB - 4.0f * fA * fC;
	if (discriminant < 0)
	{
		return false;
	}

	auto root = (-fB - sqrt(discriminant)) / (2.f * fA);
	if (root < fMin || fMax < root) 
	{
		root = (-fB + sqrt(discriminant)) / (2.f * fA);
		if (root < fMin || fMax < root)
			return false;
	}

	hitRecord.HitDistance = root;
	hitRecord.WorldPosition = ray.Origin + ray.Direction * hitRecord.HitDistance;
	hitRecord.EntityID = m_uiEntitID;
	hitRecord.MaterialID = m_uiMaterialID;
	hitRecord.SetWorldNormal(ray, (hitRecord.WorldPosition - m_vec3Position) / m_fRadius);
	return true;
}

SAND_TABLE_NAMESPACE_END
