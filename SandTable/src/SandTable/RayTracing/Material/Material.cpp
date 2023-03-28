#include "pch.h"
#include "Material.h"

SAND_TABLE_NAMESPACE_BEGIN

Material::Material(uint32_t uiMaterialID) :
	m_uiMaterialID(uiMaterialID),
	m_bDirty(false)
{
}


glm::dvec3 Material::Emitted(const glm::dvec3& SamplePoint, const glm::dvec2& UV) const
{
	return glm::dvec3(0.0);
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
