#include "pch.h"
#include "Isotropic.h"
#include "SandTable/RayTracing/Color/ColorTexture.h"
SAND_TABLE_NAMESPACE_BEGIN

Isotropic::Isotropic(Ref<ColorTexture> spColorTexture, uint32_t uiMateriaID):
	m_spColorTexture(spColorTexture),
	Material(uiMateriaID)
{
}

bool Isotropic::Scatter(const Ray& rayIn, const HitRecord& hitRecord, glm::dvec3& attenuation, Ray& rayOut) const
{
	rayOut.Origin = rayIn.Origin;
	rayOut.Direction = rayIn.Direction;
	attenuation = m_spColorTexture->GetColor(hitRecord.WorldPosition, hitRecord.UV);
	return true;
}

SAND_TABLE_NAMESPACE_END
