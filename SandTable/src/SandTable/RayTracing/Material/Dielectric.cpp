#include "pch.h"
#include "Dielectric.h"
#include "SandTable/Math/Random.h"

SAND_TABLE_NAMESPACE_BEGIN

Dielectric::Dielectric(uint32_t uiMaterialID):
	Material(uiMaterialID),
	m_fMetallic(0.f)
{
}

bool Dielectric::Scatter(const Ray& rayIn, const HitRecord& hitRecord, glm::vec3& attenuation, Ray& rayOut) const
{
	attenuation = glm::vec3(1.f);
	float fRatio = hitRecord.FrontFace ? (1.0 / m_fMetallic) : m_fMetallic;
	auto unitDir = glm::normalize(rayIn.Direction);
	auto fCosTheta = glm::min(glm::dot(-unitDir, hitRecord.WorldNormal), 1.f);
	auto fSinTheta = sqrt(1.0 - fCosTheta * fCosTheta);
	if (fSinTheta * fRatio > 1.0 || Reflectance(fCosTheta, fRatio) > Random::Float())
	{
		rayOut.Direction = glm::reflect(unitDir, hitRecord.WorldNormal) + Random::UnitSphere();
	}
	else
	{
		rayOut.Direction = glm::refract(unitDir, hitRecord.WorldNormal, fRatio);
	}

	rayOut.Origin = glm::dot(rayOut.Direction, hitRecord.WorldNormal) < 0 ? hitRecord.WorldPosition - hitRecord.WorldNormal * 0.00001f :
		hitRecord.WorldPosition + hitRecord.WorldNormal * 0.00001f;
	return true;
}

void Dielectric::SetMetallic(float fMetallic)
{
	m_fMetallic = fMetallic;
	m_bDirty = true;
}

float Dielectric::GetMetallic() const
{
	return m_fMetallic;
}

float Dielectric::Reflectance(float fCosTheta, float fRatio) const
{
	// Use Schlick's approximation for reflectance.
	auto r0 = (1 - fRatio) / (1 + fRatio);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - fCosTheta), 5);
}

SAND_TABLE_NAMESPACE_END
