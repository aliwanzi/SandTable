#include "pch.h"
#include "Material.h"

SAND_TABLE_NAMESPACE_BEGIN
Material::Material(uint32_t uiMaterialID) :
	m_vec3Albedo(glm::vec3(0.f)),
	m_fRoughness(0.f),
	m_fMetallic(0.f),
	m_uiMaterialID(uiMaterialID),
	m_bDirty(false)
{
}

void Material::SetRoughness(float fRoughness)
{
	m_fRoughness = fRoughness;
	m_bDirty = true;
}

float Material::GetRoughness() const
{
	return m_fRoughness;
}

void Material::SetMetallic(float fMetallic)
{
	m_fMetallic = fMetallic;
	m_bDirty = true;
}

float Material::GetMetallic() const
{
	return m_fMetallic;
}

void Material::SetAlbedo(const glm::vec3& vec3Albedo)
{
	m_vec3Albedo = vec3Albedo;
	m_bDirty = true;
}

const glm::vec3& Material::GetAlbedo() const
{
	return m_vec3Albedo;
}

void Material::SetMaterialID(uint32_t uiMaterialID)
{
	m_uiMaterialID = uiMaterialID;
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
