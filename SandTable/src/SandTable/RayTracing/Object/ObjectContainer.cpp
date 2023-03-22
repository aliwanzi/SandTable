#include "pch.h"
#include "ObjectContainer.h"

SAND_TABLE_NAMESPACE_BEGIN

bool ObjectContainer::Hit(const Ray& ray, float fMin, float fMax, HitRecord& hitRecord) const
{
	HitRecord hitRecordTemp;
	float fCloset = fMax;
	bool bHitAnything(false);
	for (auto& iter : m_mapObject)
	{
		if (iter.second->Hit(ray, fMin, fCloset, hitRecordTemp) && hitRecordTemp.HitDistance < fCloset)
		{
			bHitAnything = true;
			fCloset = hitRecordTemp.HitDistance;
			hitRecord = hitRecordTemp;

		}
	}
	return bHitAnything;
}

const std::map<uint32_t, std::shared_ptr<Object>>& ObjectContainer::GetAllObject() const
{
	return m_mapObject;
}

void ObjectContainer::AddObject(std::shared_ptr<Object> spObject)
{
	m_mapObject.insert(std::make_pair(spObject->GetEntityID(), spObject));
}

void ObjectContainer::Clear()
{
	m_mapObject.clear();
}

SAND_TABLE_NAMESPACE_END