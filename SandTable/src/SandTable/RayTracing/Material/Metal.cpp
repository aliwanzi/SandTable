#include "pch.h"
#include "Metal.h"

SAND_TABLE_NAMESPACE_BEGIN

Metal::Metal(uint32_t uiMaterialID) :
	Material(uiMaterialID),
	m_vec3Albedo(glm::dvec3(0.f)),
	m_fRoughness(0.f)
{
}

bool Metal::Scatter(const Ray& rayIn, const HitRecord& hitRecord, ScatterRecord& scatterRecord) const
{
	scatterRecord.Attenuation = m_vec3Albedo;
	scatterRecord.SpecularRay.Step = rayIn.Step;
	scatterRecord.SpecularRay.Origin = hitRecord.WorldPosition;
	scatterRecord.SpecularRay.Direction = glm::reflect(rayIn.Direction, hitRecord.WorldNormal + m_fRoughness * Random::UnitSphere());

	return glm::dot(scatterRecord.SpecularRay.Direction, hitRecord.WorldNormal) > 0;
}

void Metal::SetAlbedo(const glm::dvec3& vec3Albedo)
{
	m_vec3Albedo = vec3Albedo;
	m_bDirty = true;
}

const glm::dvec3& Metal::GetAlbedo() const
{
	return m_vec3Albedo;
}

void Metal::SetRoughness(double fRoughness)
{
	m_bDirty = true;
	m_fRoughness = fRoughness < 1 ? fRoughness : 1;
}

float Metal::GetRoughness() const
{
	return m_fRoughness;
}

SAND_TABLE_NAMESPACE_END