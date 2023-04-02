#include "pch.h"
#include "ConstantMedium.h"
SAND_TABLE_NAMESPACE_BEGIN

ConstantMedium::ConstantMedium(Ref<Object> spObject, double dDensity):
	m_spObject(spObject),
	m_dDensity(-1.0/dDensity)
{

}

bool ConstantMedium::Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord) const
{
	HitRecord ht1, ht2;
	if (!m_spObject->Hit(ray, -Random::FloatMax(), Random::FloatMax(), ht1))
		return false;
	if (!m_spObject->Hit(ray, ht1.HitDistance + 0.001, Random::FloatMax(), ht2))
		return false;

	ht1.HitDistance = glm::max(ht1.HitDistance, fMin);
	ht2.HitDistance = glm::min(ht2.HitDistance, fMax);

	if (ht1.HitDistance >= ht2.HitDistance)
	{
		return false;
	}
	ht1.HitDistance = glm::max(ht1.HitDistance, 0.0);

	const auto rayLength = glm::length(ray.Direction);
	const auto hitDistance = (ht2.HitDistance - ht1.HitDistance) * rayLength;
	const auto distance = m_dDensity * glm::log(Random::Float());
	if (distance > hitDistance)
	{
		return false;
	}
	hitRecord.HitDistance = ht1.HitDistance + distance / rayLength;
	hitRecord.WorldPosition = ray.Origin + hitRecord.HitDistance * ray.Direction;
	hitRecord.WorldNormal = glm::dvec3(1, 0, 0);
	hitRecord.FrontFace = true;
	hitRecord.EntityID = m_spObject->GetEntityID();
	hitRecord.MaterialID = m_spObject->GetMaterialID();
	return true;
}

bool ConstantMedium::CreateBoundingBox(double dStepBegin, double dStepEnd)
{
	return m_spObject->CreateBoundingBox(dStepBegin, dStepEnd);
}


SAND_TABLE_NAMESPACE_END