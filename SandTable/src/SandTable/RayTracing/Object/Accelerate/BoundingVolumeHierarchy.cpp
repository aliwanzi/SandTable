#include "pch.h"
#include "BoundingVolumeHierarchy.h"
SAND_TABLE_NAMESPACE_BEGIN

BoundingVolumeHierarchy::BoundingVolumeHierarchy(
	Ref<ObjectContainer> spObjectContainer, uint32_t uiEntitID):
	Object(uiEntitID),
	m_spObjectContainer(spObjectContainer),
	m_spLeft(nullptr),
	m_spRight(nullptr)
{
	auto& objects = m_spObjectContainer->GetAllObject();
	for (auto& iter : objects)
	{
		iter->CreateBoundingBox(0, 1);
	}
	CreateBVHNode(objects);
}

BoundingVolumeHierarchy::BoundingVolumeHierarchy():
	m_spObjectContainer(nullptr),
	m_spLeft(nullptr),
	m_spRight(nullptr)
{
}

bool BoundingVolumeHierarchy::Hit(const Ray& ray, 
	double fMin,
	double fMax,
	HitRecord& hitRecord)
{
	Ray transRay;
	transRay.Origin = m_spTransform->GetRotationInverse() * m_spTransform->GetTranslationInverse()
		* glm::dvec4(ray.Origin, 1.0);
	transRay.Direction = m_spTransform->GetRotationInverse() * glm::dvec4(ray.Direction, 1.0);

	if (!m_spBoundingBox->Intersect(transRay, fMin, fMax))
	{
		return false;
	}
	bool hitLeft = m_spLeft->Hit(transRay, fMin, fMax, hitRecord);
	bool hitRight = m_spRight->Hit(transRay, fMin, hitLeft ? hitRecord.HitDistance : fMax, hitRecord);

	if (hitLeft || hitRight)
	{
		hitRecord.WorldPosition = m_spTransform->GetTranslation() * m_spTransform->GetRotation() * glm::dvec4(hitRecord.WorldPosition, 1.0);
		hitRecord.WorldNormal = m_spTransform->GetRotation() * glm::dvec4(hitRecord.WorldNormal, 1.0);
		return true;
	}
	return false;
}

void BoundingVolumeHierarchy::SetMaterialID(uint32_t uiMaterialID)
{
	Object::SetMaterialID(uiMaterialID);
	auto& objects = m_spObjectContainer->GetAllObject();
	for (auto& object : objects)
	{
		object->SetMaterialID(uiMaterialID);
	}
}

bool BoundingVolumeHierarchy::CreateBoundingBox(double dStepBegin, double dStepEnd)
{
	return true;
}


void BoundingVolumeHierarchy::CreateBVHNode(std::vector<Ref<Object>>& vecObject)
{
	for (auto& iter : vecObject)
	{
		m_spBoundingBox->Merge(iter->GetBoundingBox());
	}

	if (1 == vecObject.size())
	{
		m_spLeft = m_spRight = vecObject[0];
		m_spBoundingBox->Merge(m_spLeft->GetBoundingBox());
	}
	else if (2 == vecObject.size())
	{
		m_spLeft = vecObject[0];
		m_spRight = vecObject[1];
	}
	else
	{
		int iMaxExtent = m_spBoundingBox->GetMaxExtent();
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
					return obj1->GetBoundingBox()->GetCentroid().z < obj2->GetBoundingBox()->GetCentroid().z;
				});
			break;
		}
		}

		auto beginning = vecObject.begin();
		auto middling = vecObject.begin() + (vecObject.size() / 2);
		auto ending = vecObject.end();

		auto leftshapes = std::vector<Ref<Object>>(beginning, middling);
		auto rightshapes = std::vector<Ref<Object>>(middling, ending);

		SAND_TABLE_ASSERT(vecObject.size() == leftshapes.size() + rightshapes.size(), "Create BVH out of size");

		m_spLeft = CreateRef<BoundingVolumeHierarchy>();
		auto spLeft = std::dynamic_pointer_cast<BoundingVolumeHierarchy>(m_spLeft);
		spLeft->CreateBVHNode(leftshapes);

		m_spRight = CreateRef<BoundingVolumeHierarchy>();
		auto spRight = std::dynamic_pointer_cast<BoundingVolumeHierarchy>(m_spRight);
		spRight->CreateBVHNode(rightshapes);
	}
}

SAND_TABLE_NAMESPACE_END