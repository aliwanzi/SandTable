#include "pch.h"
#include "Object.h"

SAND_TABLE_NAMESPACE_BEGIN
Object::Object(uint32_t uiEntitID):
	m_uiEntitID(uiEntitID),
	m_uiMaterialID(0),
	m_bDirty(false),
	m_spTransform(CreateRef<Transform>())
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
	m_spTransform->SetRotateY(glm::radians(angle));
}

const Ref<BoundingBox>& Object::GetBoundingBox() const
{
	SAND_TABLE_ASSERT(m_spBoundingBox != nullptr, "Bounding Box is null");
	return m_spBoundingBox;
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