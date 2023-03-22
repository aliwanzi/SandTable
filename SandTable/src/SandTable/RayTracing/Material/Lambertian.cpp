#include "pch.h"
#include "Lambertian.h"
#include "SandTable/RayTracing/Hittable.h"
#include "SandTable/Math/Random.h"

SAND_TABLE_NAMESPACE_BEGIN

Lambertian::Lambertian(uint32_t uiMaterialID) :
	Material(uiMaterialID),
	m_vec3Albedo(0.f)
{
}

bool Lambertian::Scatter(const Ray& rayIn, const HitRecord& hitRecord, glm::dvec3& attenuation, Ray& rayOut) const
{
	attenuation = m_vec3Albedo;
	rayOut.Step = rayIn.Step;
	rayOut.Origin = glm::dot(rayOut.Direction, hitRecord.WorldNormal) < 0 ? hitRecord.WorldPosition - hitRecord.WorldNormal * 0.00001 :
		hitRecord.WorldPosition + hitRecord.WorldNormal * 0.00001;
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

SAND_TABLE_NAMESPACE_END
