#include "pch.h"
#include "Material.h"
#include "SandTable/Math/Random.h"

SAND_TABLE_NAMESPACE_BEGIN

Material::Material(uint32_t uiMaterialID) :
	m_uiMaterialID(uiMaterialID),
	m_bDirty(false)
{
}


uint32_t Material::GetMaterialID() const
{
	return m_uiMaterialID;
}

void Material::ResetDirty()
{
	m_bDirty = false;
}

bool Material::GetDirty()
{
	return m_bDirty;
}

SAND_TABLE_NAMESPACE_END
