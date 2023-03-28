#include "pch.h"
#include "ObjectContainer.h"

SAND_TABLE_NAMESPACE_BEGIN

ObjectContainer::ObjectContainer():
	m_bInitilize(false)
{
}

bool ObjectContainer::Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord)
{
	if (!m_bInitilize)
	{
		if (!CreateBoundingBox(fMin, fMax))
		{
			LOG_DEV_ERROR("Create BoundingBox Failed");
			return false;
		}
	}


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
	}
	m_bInitilize = true;
	return true;
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