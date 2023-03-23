#include "pch.h"
#include "BoundingVolumeHierarchy.h"
#include "SandTable/Math/Random.h"
SAND_TABLE_NAMESPACE_BEGIN

BoundingVolumeHierarchy::BoundingVolumeHierarchy(
	std::shared_ptr<ObjectContainer> spObjectContainer, 
	double dStepBegin, 
	double dStepEnd):
	BoundingVolumeHierarchy(
		spObjectContainer->GetAllObject(), 
		0,
		spObjectContainer->GetAllObject().size(),
		dStepBegin, 
		dStepEnd)
{
}


BoundingVolumeHierarchy::BoundingVolumeHierarchy(
	std::vector<Ref<Object>>& vecObject, 
	size_t start, size_t end, 
	double dStepBegin, double dStepEnd)
{
	size_t objectSpan = end - start;
	if (1 == objectSpan)
	{
		m_spLeft = m_spRight = vecObject[0];
	}
	else if (objectSpan == 2)
	{
		m_spLeft = vecObject[0];
		m_spRight = vecObject[1];
	}
	else
	{
		auto spBoundingBox = CreateRef<BoundingBox>();
		for (auto& iter : vecObject)
		{
			spBoundingBox->Merge(iter->GetBoundingBox());
		}
		int iMaxExtent = spBoundingBox->GetMaxExtent();
		switch (iMaxExtent)
		{
			case 0:
			{
				std::sort(vecObject.begin(), vecObject.end(), [](auto obj1, auto obj2)
					{
						return obj1->GetBoundingBox()->GetCentroid().x < obj2->GetBoundingBox()->GetCentroid().x;
					});
				break;
			}
			case 1:
			{
				std::sort(vecObject.begin(), vecObject.end(), [](auto obj1, auto obj2)
					{
						return obj1->GetBoundingBox()->GetCentroid().y < obj2->GetBoundingBox()->GetCentroid().y;
					});
				break;
			}
			case 2:
			{
				std::sort(vecObject.begin(), vecObject.end(), [](auto obj1, auto obj2)
					{
						return obj1->GetBoundingBox()->GetCentroid().z< obj2->GetBoundingBox()->GetCentroid().z;
					});
				break;
			}
		}
		size_t middling = start + objectSpan / 2;

		m_spLeft = std::make_shared<BoundingVolumeHierarchy>(vecObject, start, middling, dStepBegin, dStepEnd);
		m_spRight = std::make_shared<BoundingVolumeHierarchy>(vecObject, middling, end, dStepBegin, dStepEnd);
	}

	auto success = m_spLeft->CreateBoundingBox(dStepBegin, dStepEnd);
	success &= m_spRight->CreateBoundingBox(dStepBegin, dStepEnd);
	SAND_TABLE_ASSERT(success,"Creat Bounding Box Failed");

	//m_spBoundingBox = CreateRef<BoundingBox>(dStepBegin, dStepEnd);
	m_spBoundingBox->SurrondingBox(m_spLeft->GetBoundingBox(), m_spRight->GetBoundingBox());
}


bool BoundingVolumeHierarchy::Hit(const Ray& ray, 
	float fMin,
	float fMax, 
	HitRecord& hitRecord) const
{
	return false;
}

bool BoundingVolumeHierarchy::CreateBoundingBox(double dStepBegin, double dStepEnd)
{
	//m_spBoundingBox = CreateRef<BoundingBox>();
	return true;
}



SAND_TABLE_NAMESPACE_END