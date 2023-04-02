#include "pch.h"
#include "Material.h"

SAND_TABLE_NAMESPACE_BEGIN

Material::Material(uint32_t uiMaterialID) :
	m_uiMaterialID(uiMaterialID),
	m_bDirty(false)
{
}


bool Material::Scatter(const Ray& rayIn, const HitRecord& hitRecord, ScatterRecord& scatterRecord) const
{
	return false;
}

double Material::ScatterPDF(const Ray& rayIn, const HitRecord& hitRecord, Ray& rayOut) const
{
	return 0;
}

glm::dvec3 Material::Emitted(const HitRecord& hitRecord) const
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
