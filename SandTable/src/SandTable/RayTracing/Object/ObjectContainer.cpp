#include "pch.h"
#include "ObjectContainer.h"

SAND_TABLE_NAMESPACE_BEGIN

bool ObjectContainer::Hit(const Ray& ray, float fMin, float fMax, HitRecord& hitRecord) const
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

std::vector<Ref<Object>>& ObjectContainer::GetAllObject()
{
	return m_vecObject;
}

void ObjectContainer::AddObject(std::shared_ptr<Object> spObject)
{
	m_vecObject.emplace_back(spObject);
}

void ObjectContainer::Clear()
{
	m_vecObject.clear();
}

SAND_TABLE_NAMESPACE_END