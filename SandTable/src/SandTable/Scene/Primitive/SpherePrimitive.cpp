#include "pch.h"
#include "SpherePrimitive.h"

SAND_TABLE_NAMESPACE_BEGIN
namespace
{
	const float fHitDistance = std::numeric_limits<float>::max();
}
SpherePrimitive::SpherePrimitive(const glm::vec3& vec3Position, float fRadius, uint32_t uiEntitID) :
	m_vec3Position(vec3Position),
	m_fRadius(fRadius),
	m_uiEntitID(uiEntitID),
	m_uiMaterialID(-1),
	m_bDirty(false)
{
}

void SpherePrimitive::SetPosition(const glm::vec3& vec3Position)
{
	m_vec3Position = vec3Position;
	m_bDirty = true;
}

const glm::vec3& SpherePrimitive::GetPosition() const
{
	return m_vec3Position;
}

void SpherePrimitive::SetRadius(float fRadius)
{
	m_fRadius = fRadius;
	m_bDirty = true;
}

float SpherePrimitive::GetRadius() const
{
	return m_fRadius;
}

void SpherePrimitive::SetMaterialID(uint32_t uiMaterialID)
{
	m_uiMaterialID = uiMaterialID;
	m_bDirty = true;
}

uint32_t SpherePrimitive::GetMaterialID() const
{
	return m_uiMaterialID;
}

void SpherePrimitive::SetEntityID(uint32_t uiEntityID)
{
	m_uiEntitID = uiEntityID;
	m_bDirty = true;
}

uint32_t SpherePrimitive::GetEntityID() const
{
	return m_uiEntitID;
}

void SpherePrimitive::ResetDirty()
{
	m_bDirty = false;
}

bool SpherePrimitive::GetDirty()
{
	return m_bDirty;
}

bool SpherePrimitive::Hit(const Ray& ray, HitPayLoad& hitPayLoad)
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
	hitPayLoad.HitDistance = root;
	hitPayLoad.WorldPosition = ray.Origin + ray.Direction * hitPayLoad.HitDistance;
	hitPayLoad.EntityID = m_uiEntitID;
	hitPayLoad.SetWorldNormal(ray, (hitPayLoad.WorldPosition - m_vec3Position) / m_fRadius);
	return true;
}

SAND_TABLE_NAMESPACE_END
