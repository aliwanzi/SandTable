#include "pch.h"
#include "Lambertian.h"
#include "SandTable/RayTracing/Hittable.h"
#include "SandTable/RayTracing/Color/SolidColorTexture.h"
#include "SandTable/RayTracing/PDF/ONB.h"

SAND_TABLE_NAMESPACE_BEGIN

Lambertian::Lambertian(uint32_t uiMaterialID) :
	Material(uiMaterialID),
	m_spColorTexture(nullptr)
{
}

bool Lambertian::Scatter(const Ray& rayIn, const HitRecord& hitRecord, glm::dvec3& attenuation, Ray& rayOut, double& pdf) const
{
	ONB onb(hitRecord.WorldNormal);
	auto direction = onb.Local(Random::CosineDirection());

	attenuation = m_spColorTexture->GetColor(hitRecord.WorldPosition, hitRecord.UV);
	rayOut.Step = rayIn.Step;
	rayOut.Origin = hitRecord.WorldPosition;
	rayOut.Direction = glm::normalize(direction);

	pdf = glm::dot(onb.GetW(), glm::normalize(rayOut.Direction)) / glm::pi<double>();
	return true;
}

double Lambertian::ScatterPDF(const Ray& rayIn, const HitRecord& hitRecord,Ray& rayOut) const
{
	auto consine = glm::dot(glm::normalize(hitRecord.WorldNormal), glm::normalize(rayOut.Direction));
	return consine < 0 ? 0 : consine / glm::pi<double>();
}

void Lambertian::SetAlbedo(const glm::dvec3& vec3Albedo)
{
	m_bDirty = true;
	m_spColorTexture = CreateRef<SolidColorTexture>(vec3Albedo);
}

const glm::dvec3& Lambertian::GetAlbedo() const
{
	return m_spColorTexture->GetColor(glm::dvec3(0.f), glm::dvec2(0.0));
}

void Lambertian::SetColorTexture(Ref<ColorTexture> spColorTexture)
{
	m_spColorTexture = spColorTexture;
}

SAND_TABLE_NAMESPACE_END
