#include "pch.h"
#include "Metal.h"
#include "SandTable/Math/Random.h"

SAND_TABLE_NAMESPACE_BEGIN

Metal::Metal(uint32_t uiMaterialID) :
	Material(uiMaterialID),
	m_vec3Albedo(glm::vec3(0.f)),
	m_fRoughness(0.f)
{
}

bool Metal::Scatter(const Ray& rayIn, const HitRecord& hitRecord, glm::vec3& attenuation, Ray& rayOut) const
{
	attenuation = m_vec3Albedo;
	rayOut.Origin = glm::dot(rayOut.Direction, hitRecord.WorldNormal) < 0 ? hitRecord.WorldPosition - hitRecord.WorldNormal * 0.00001f :
		hitRecord.WorldPosition + hitRecord.WorldNormal * 0.00001f;
	auto vec3Mircor = m_fRoughness * Random::UnitSphere();
	rayOut.Direction = glm::reflect(glm::normalize(rayIn.Direction), hitRecord.WorldNormal);

	return glm::dot(rayOut.Direction, hitRecord.WorldNormal) > 0;
}

void Metal::SetAlbedo(const glm::vec3& vec3Albedo)
{
	m_vec3Albedo = vec3Albedo;
	m_bDirty = true;
}

const glm::vec3& Metal::GetAlbedo() const
{
	return m_vec3Albedo;
}

void Metal::SetRoughness(float fRoughness)
{
	m_bDirty = true;
	m_fRoughness = fRoughness < 1 ? fRoughness : 1;
}

float Metal::GetRoughness() const
{
	return m_fRoughness;
}

SAND_TABLE_NAMESPACE_END