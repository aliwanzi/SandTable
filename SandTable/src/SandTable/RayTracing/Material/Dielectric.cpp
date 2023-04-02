#include "pch.h"
#include "Dielectric.h"

SAND_TABLE_NAMESPACE_BEGIN

Dielectric::Dielectric(uint32_t uiMaterialID) :
	Material(uiMaterialID),
	m_fMetallic(0.f)
{
}

bool Dielectric::Scatter(const Ray& rayIn, const HitRecord& hitRecord, ScatterRecord& scatterRecord) const
{
	scatterRecord.Attenuation = glm::vec3(1.f);
	scatterRecord.SpecularRay.Step = rayIn.Step;
	double fRatio = hitRecord.FrontFace ? (1.0 / m_fMetallic) : m_fMetallic;
	auto unitDir = glm::normalize(rayIn.Direction);
	auto fCosTheta = glm::min(glm::dot(-unitDir, hitRecord.WorldNormal), 1.0);
	auto fSinTheta = sqrt(1.0 - fCosTheta * fCosTheta);
	if (fSinTheta * fRatio > 1.0 || Reflectance(fCosTheta, fRatio) > Random::Float())
	{
		scatterRecord.SpecularRay.Direction = glm::reflect(unitDir, hitRecord.WorldNormal);
	}
	else
	{
		scatterRecord.SpecularRay.Direction = glm::refract(unitDir, hitRecord.WorldNormal, fRatio);
	}

	scatterRecord.SpecularRay.Origin = hitRecord.WorldPosition;
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
