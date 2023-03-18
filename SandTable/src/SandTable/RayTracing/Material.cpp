#include "pch.h"
#include "Material.h"

SAND_TABLE_NAMESPACE_BEGIN
Material::Material():
	m_vec3Albedo(glm::vec3(0.f)),
	m_fRoughness(1.f),
	m_fMetallic(0.f)
{
}

void Material::SetRoughness(float fRoughness)
{
	m_fRoughness = fRoughness;
}

float& Material::GetRoughness()
{
	return m_fRoughness;
}

void Material::SetMetallic(float fMetallic)
{
	m_fMetallic = fMetallic;
}

float& Material::GetMetallic()
{
	return m_fMetallic;
}

void Material::SetAlbedo(const glm::vec3& vec3Albedo)
{
	m_vec3Albedo = vec3Albedo;
}

glm::vec3& Material::GetAlbedo()
{
	return m_vec3Albedo;
}

SAND_TABLE_NAMESPACE_END
