#include "pch.h"
#include "Box.h"
#include "Rectangle/XYRectangle.h"
#include "Rectangle/XZRectangle.h"
#include "Rectangle/YZRectangle.h"

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
	spYZRectangle1->SetXPos(PointMin.x);

	m_spObjectContainer->AddObject(spXYRectangle0);
	m_spObjectContainer->AddObject(spXYRectangle1);
	m_spObjectContainer->AddObject(spXZRectangle0);
	m_spObjectContainer->AddObject(spXZRectangle1);
	m_spObjectContainer->AddObject(spYZRectangle0);
	m_spObjectContainer->AddObject(spYZRectangle1);
}


bool Box::Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord)
{
	return m_spObjectContainer->Hit(ray, fMin, fMax, hitRecord);
}

bool Box::CreateBoundingBox(double dStepBegin, double dStepEnd)
{
	auto& objects = m_spObjectContainer->GetAllObject();
	for (auto& object : objects)
	{
		object->SetTranslate(m_spTransform->GetTranslate());
	}

	if (!m_spObjectContainer->CreateBoundingBox(dStepBegin, dStepEnd))
	{
		return false;
	}
	m_spBoundingBox = m_spObjectContainer->GetBoundingBox();

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
