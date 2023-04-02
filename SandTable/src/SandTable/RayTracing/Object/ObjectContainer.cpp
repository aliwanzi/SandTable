#include "pch.h"
#include "ObjectContainer.h"

SAND_TABLE_NAMESPACE_BEGIN

ObjectContainer::ObjectContainer()
{
}

bool ObjectContainer::Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord) const
{
	HitRecord hitRecordTemp;
	float fCloset = fMax;
	bool bHitAnything(false);
	for (auto& iter : m_vecObject)
	{
		if (iter->Hit(ray, fMin, fCloset, hitRecordTemp) && hitRecordTemp.HitDistance < fCloset)
		{
			bHitAnything = true;
			fCloset = hitRecordTemp.HitDistance;
			hitRecord = hitRecordTemp;

		}
	}
	return bHitAnything;
}

bool ObjectContainer::CreateBoundingBox(double dStepBegin, double dStepEnd)
{
	for (auto& iter : m_vecObject)
	{
		if (!iter->CreateBoundingBox(dStepBegin,dStepEnd))
		{
			return false;
		}
		m_spBoundingBox->Merge(iter->GetBoundingBox());
	}
	return true;
}

glm::dvec3 ObjectContainer::SampleDirection(const glm::dvec3& vec3HitPoint) const
{
	auto objectSize = static_cast<int>(m_vecObject.size());
	int object = glm::clamp(Random::Uint(0, objectSize - 1), 0, objectSize - 1);
	return m_vecObject[object]->SampleDirection(vec3HitPoint);
}

double ObjectContainer::GetPDF(const glm::dvec3& vec3HitPoint, const glm::dvec3& direction) const
{
	double dSumPDF(0.0);
	for (const auto& object : m_vecObject)
	{
		dSumPDF += object->GetPDF(vec3HitPoint, direction);
	}
	return dSumPDF / (m_vecObject.size());
}

std::vector<Ref<Object>>& ObjectContainer::GetAllObject()
{
	return m_vecObject;
}

void ObjectContainer::AddObject(Ref<Object> spObject)
{
	m_vecObject.emplace_back(spObject);
}

void ObjectContainer::Clear()
{
	m_vecObject.clear();
}

SAND_TABLE_NAMESPACE_END