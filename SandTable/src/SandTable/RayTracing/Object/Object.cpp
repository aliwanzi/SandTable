#include "pch.h"
#include "Object.h"

SAND_TABLE_NAMESPACE_BEGIN
Object::Object(uint32_t uiEntitID):
	m_uiEntitID(uiEntitID),
	m_uiMaterialID(0),
	m_bDirty(false)
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