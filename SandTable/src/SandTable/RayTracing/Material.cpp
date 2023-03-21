#include "pch.h"
#include "Material.h"
#include "SandTable/Math/Random.h"

SAND_TABLE_NAMESPACE_BEGIN
namespace
{
	float reflectance(double fCosTheta, double fRatio) {
		// Use Schlick's approximation for reflectance.
		auto r0 = (1 - fRatio) / (1 + fRatio);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - fCosTheta), 5);
	}
}

Material::Material(MaterialType eMaterialType, uint32_t uiMaterialID) :
	m_materialType(eMaterialType),
	m_uiMaterialID(uiMaterialID),
	m_vec3Albedo(glm::vec3(0.f)),
	m_fRoughness(0.f),
	m_fMetallic(0.f),
	m_bDirty(false)
{
}

void Material::SetRoughness(float fRoughness)
{
	m_fRoughness = fRoughness;
	m_bDirty = true;
}

float Material::GetRoughness() const
{
	return m_fRoughness;
}

void Material::SetMetallic(float fMetallic)
{
	m_fMetallic = fMetallic;
	m_bDirty = true;
}

float Material::GetMetallic() const
{
	return m_fMetallic;
}

void Material::SetAlbedo(const glm::vec3& vec3Albedo)
{
	m_vec3Albedo = vec3Albedo;
	m_bDirty = true;
}

const glm::vec3& Material::GetAlbedo() const
{
	return m_vec3Albedo;
}

void Material::SetMaterialID(uint32_t uiMaterialID)
{
	m_uiMaterialID = uiMaterialID;
}

uint32_t Material::GetMaterialID() const
{
	return m_uiMaterialID;
}

void Material::ResetDirty()
{
	m_bDirty = false;
}

bool Material::GetDirty()
{
	return m_bDirty;
}

bool Material::Scatter(HitPayLoad& hitPayload, Ray& ray, glm::vec3& color)
{
	switch (m_materialType)
	{
		case MaterialType::Lambertian:
		{
			color = m_vec3Albedo;
			ray.Direction = hitPayload.WorldNormal + Random::Vec3();
			return true;
		}
		case MaterialType::Metal:
		{
			color = m_vec3Albedo;
			ray.Origin = hitPayload.WorldPosition + hitPayload.WorldNormal * 0.0001f;
			auto vec3Mircor = m_fRoughness * Random::Vec3();
			ray.Direction = glm::reflect(ray.Direction, hitPayload.WorldNormal + vec3Mircor);
			return true;
		}
		case MaterialType::Dielectric:
		{
			color = glm::vec3(1.f);
			float fRatio = hitPayload.FrontFace ? (1.0 / m_fMetallic) : m_fMetallic;
			auto unitDir = glm::normalize(ray.Direction);
			auto fCosTheta = glm::min(glm::dot(-unitDir, hitPayload.WorldNormal), 1.f);
			auto fSinTheta = sqrt(1.0 - fCosTheta * fCosTheta);
			if (fSinTheta * fRatio > 1.0 || reflectance(fCosTheta, fRatio)>Random::Float())
			{
				ray.Direction = glm::reflect(unitDir, hitPayload.WorldNormal);
			}
			else
			{
				ray.Direction = glm::refract(unitDir, hitPayload.WorldNormal, fRatio);
			}
			return true;
		}
		default:
		{
			SAND_TABLE_ASSERT(false, "Unknown Material type");
			return false;
		}
	}
	return false;
}

SAND_TABLE_NAMESPACE_END
