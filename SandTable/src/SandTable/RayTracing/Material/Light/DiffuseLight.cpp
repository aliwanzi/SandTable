#include "pch.h"
#include "DiffuseLight.h"

SAND_TABLE_NAMESPACE_BEGIN
DiffuseLight::DiffuseLight(Ref<ColorTexture> spColorTexture, uint32_t uiMaterialID) :
	Material(uiMaterialID),
	m_spColorTexture(spColorTexture)
{
}
bool DiffuseLight::Scatter(const Ray& rayIn, const HitRecord& hitRecord, ScatterRecord& scatterRecord) const
{
	return false;
}
glm::dvec3 DiffuseLight::Emitted(const HitRecord& hitRecord) const
{
	return hitRecord.FrontFace ? m_spColorTexture->GetColor(hitRecord.WorldPosition, hitRecord.UV) : glm::dvec3(0.0);
}
SAND_TABLE_NAMESPACE_END
