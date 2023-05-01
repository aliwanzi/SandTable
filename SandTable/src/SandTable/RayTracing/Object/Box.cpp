#include "pch.h"
#include "Box.h"
#include "Rectangle/XYRectangle.h"
#include "Rectangle/XZRectangle.h"
#include "Rectangle/YZRectangle.h"
#include "Triangle.h"

SAND_TABLE_NAMESPACE_BEGIN

Box::Box(const glm::dvec3& PointMin, const glm::dvec3& PointMax, uint32_t uiEntityID):
	Object(uiEntityID),
	m_spObjectContainer(CreateRef<ObjectContainer>())
{
	auto spXYRectangle0 = CreateRef<XYRectangle>(uiEntityID);
	spXYRectangle0->SetXYPoints(glm::dvec2(PointMin.x, PointMin.y), glm::dvec2(PointMax.x, PointMax.y));
	spXYRectangle0->SetZPos(PointMin.z);
	auto spXYRectangle1 = CreateRef<XYRectangle>(uiEntityID);
	spXYRectangle1->SetXYPoints(glm::dvec2(PointMin.x, PointMin.y), glm::dvec2(PointMax.x, PointMax.y));
	spXYRectangle1->SetZPos(PointMax.z);

	auto spXZRectangle0 = CreateRef<XZRectangle>(uiEntityID);
	spXZRectangle0->SetXZPoints(glm::dvec2(PointMin.x, PointMin.z), glm::dvec2(PointMax.x, PointMax.z));
	spXZRectangle0->SetYPos(PointMin.y);
	auto spXZRectangle1 = CreateRef<XZRectangle>(uiEntityID);
	spXZRectangle1->SetXZPoints(glm::dvec2(PointMin.x, PointMin.z), glm::dvec2(PointMax.x, PointMax.z));
	spXZRectangle1->SetYPos(PointMax.y);

	auto spYZRectangle0 = CreateRef<YZRectangle>(uiEntityID);
	spYZRectangle0->SetYZPoints(glm::dvec2(PointMin.y, PointMin.z), glm::dvec2(PointMax.y, PointMax.z));
	spYZRectangle0->SetXPos(PointMin.x);
	auto spYZRectangle1 = CreateRef<YZRectangle>(uiEntityID);
	spYZRectangle1->SetYZPoints(glm::dvec2(PointMin.y, PointMin.z), glm::dvec2(PointMax.y, PointMax.z));
	spYZRectangle1->SetXPos(PointMax.x);

	m_spObjectContainer->AddObject(spXYRectangle0);
	m_spObjectContainer->AddObject(spXYRectangle1);
	m_spObjectContainer->AddObject(spXZRectangle0);
	m_spObjectContainer->AddObject(spXZRectangle1);
	m_spObjectContainer->AddObject(spYZRectangle0);
	m_spObjectContainer->AddObject(spYZRectangle1);

	Triangle triangle;
}


bool Box::Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord) const
{
	Ray transRay;
	transRay.Origin = m_spTransform->GetRotationInverse() * m_spTransform->GetTranslationInverse()
		* glm::dvec4(ray.Origin, 1.0);
	transRay.Direction = m_spTransform->GetRotationInverse() * glm::dvec4(ray.Direction, 1.0);

	if (m_spObjectContainer->Hit(transRay, fMin, fMax, hitRecord))
	{
		hitRecord.WorldPosition = m_spTransform->GetTranslation() * m_spTransform->GetRotation() * glm::dvec4(hitRecord.WorldPosition, 1.0);
		hitRecord.WorldNormal = m_spTransform->GetRotation() * glm::dvec4(hitRecord.WorldNormal, 1.0);
		return true;
	}
	return false;
}

bool Box::CreateBoundingBox(double dStepBegin, double dStepEnd)
{
	auto& objects = m_spObjectContainer->GetAllObject();
	for (auto& object : objects)
	{
		object->SetTranslate(m_spTransform->GetTranslate());
		object->SetRotateY(m_spTransform->GetRotateY());
	}

	if (!m_spObjectContainer->CreateBoundingBox(dStepBegin, dStepEnd))
	{
		return false;
	}
	m_spBoundingBox->Merge(m_spObjectContainer->GetBoundingBox());
	return true;
}

void Box::SetMaterialID(uint32_t uiMaterialID)
{
	Object::SetMaterialID(uiMaterialID);
	auto& objects = m_spObjectContainer->GetAllObject();
	for (auto& object : objects)
	{
		object->SetMaterialID(uiMaterialID);
	}
}

SAND_TABLE_NAMESPACE_END
