#pragma once
#include "SandTable/RayTracing/Ray.h"

SAND_TABLE_NAMESPACE_BEGIN

enum class MaterialType
{
	Lambertian = 1,
	Metal,
	Dielectric
};

class Material
{
public:
	Material(MaterialType eMaterialType, uint32_t uiMaterialID);

	void SetRoughness(float fRoughness);
	float GetRoughness() const;

	void SetMetallic(float fMetallic);
	float GetMetallic() const;

	void SetAlbedo(const glm::vec3& vec3Albedo);
	const glm::vec3& GetAlbedo() const;

	void SetMaterialID(uint32_t uiMaterialID);
	uint32_t GetMaterialID() const;

	void ResetDirty();
	bool GetDirty();

	bool Scatter(HitPayLoad& hitPayload, Ray& ray, glm::vec3& color);
private:
	MaterialType m_materialType;
	uint32_t m_uiMaterialID;
	float m_fRoughness;
	float m_fMetallic;
	glm::vec3 m_vec3Albedo;
	bool m_bDirty;
};

using MapMaterial = std::map<uint32_t, Ref<Material>>;
SAND_TABLE_NAMESPACE_END

