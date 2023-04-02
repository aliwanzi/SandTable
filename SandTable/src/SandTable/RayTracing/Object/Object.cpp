#include "pch.h"
#include "Object.h"

SAND_TABLE_NAMESPACE_BEGIN
Object::Object():
	m_uiEntitID(0),
	m_uiMaterialID(0),
	m_bDirty(false),
	m_spTransform(CreateRef<Transform>()),
	m_spBoundingBox(CreateRef<BoundingBox>())
{
}

Object::Object(uint32_t uiEntitID):
	m_uiEntitID(uiEntitID),
	m_uiMaterialID(0),
	m_bDirty(false),
	m_spTransform(CreateRef<Transform>()),
	m_spBoundingBox(CreateRef<BoundingBox>())
{
}

void Object::SetMaterialID(uint32_t uiMaterialID)
{
	m_uiMaterialID = uiMaterialID;
	m_bDirty = true;
}

uint32_t Object::GetMaterialID() const
{
	return m_uiMaterialID;
}

void Object::SetTranslate(const glm::dvec3& vec3Translate)
{
	m_spTransform->SetTranslate(vec3Translate);
}

void Object::SetRotateY(double angle)
{
	m_spTransform->SetRotateY(angle);
}

const Ref<BoundingBox>& Object::GetBoundingBox()
{
	SAND_TABLE_ASSERT(m_spBoundingBox != nullptr, "Bounding Box is null");
	return m_spBoundingBox;
}

glm::dvec3 Object::SampleDirection(const glm::dvec3& vec3HitPoint) const
{
	return glm::dvec3(1.0, 0.0, 0.0);
}

double Object::GetPDF(const glm::dvec3& vec3HitPoint, const glm::dvec3& direction) const
{
	return 0.0;
}

bool Object::Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord) const
{
	return false;
}

uint32_t Object::GetEntityID() const
{
	return m_uiEntitID;
}

void Object::ResetDirty()
{
	m_bDirty = false;
}

bool Object::GetDirty()
{
	return m_bDirty;
}

SAND_TABLE_NAMESPACE_END