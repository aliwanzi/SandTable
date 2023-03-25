#include "pch.h"
#include "DiffuseLight.h"

SAND_TABLE_NAMESPACE_BEGIN
DiffuseLight::DiffuseLight(Ref<ColorTexture> spColorTexture, uint32_t uiMaterialID):
    Material(uiMaterialID),
    m_spColorTexture(spColorTexture)
{
}
bool DiffuseLight::Scatter(const Ray& rayIn, const HitRecord& hitRecord, glm::dvec3& attenuation, Ray& rayOut) const
{
    return false;
}
glm::dvec3 DiffuseLight::Emitted(const glm::dvec3& SamplePoint, const glm::dvec2& UV) const
{
    return m_spColorTexture->GetColor(SamplePoint, UV);
}
SAND_TABLE_NAMESPACE_END
