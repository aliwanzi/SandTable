#include "pch.h"
#include "Lambertian.h"
#include "SandTable/RayTracing/Hittable.h"
#include "SandTable/RayTracing/Color/ColorTexture.h"

SAND_TABLE_NAMESPACE_BEGIN

Lambertian::Lambertian(uint32_t uiMaterialID) :
	Material(uiMaterialID),
	m_vec3Albedo(0.f),
	m_spColorTexture(nullptr)
{
}

bool Lambertian::Scatter(const Ray& rayIn, const HitRecord& hitRecord, glm::dvec3& attenuation, Ray& rayOut) const
{
	if (m_spColorTexture != nullptr)
	{
		attenuation = m_spColorTexture->GetColor(hitRecord.WorldPosition, hitRecord.UV);
	}
	else
	{
		attenuation = m_vec3Albedo;
	}

	rayOut.Step = rayIn.Step;
	rayOut.Origin = hitRecord.WorldPosition;
	rayOut.Direction = hitRecord.WorldNormal + Random::UnitSphere();
	return true;
}

void Lambertian::SetAlbedo(const glm::dvec3& vec3Albedo)
{
	m_bDirty = true;
	m_vec3Albedo = vec3Albedo;
}

const glm::dvec3& Lambertian::GetAlbedo() const
{
	return m_vec3Albedo;
}

void Lambertian::SetColorTexture(Ref<ColorTexture> spColorTexture)
{
	m_spColorTexture = spColorTexture;
}

SAND_TABLE_NAMESPACE_END
