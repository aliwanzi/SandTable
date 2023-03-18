#pragma once

SAND_TABLE_NAMESPACE_BEGIN
class Material
{
public:
	Material();

	void SetRoughness(float fRoughness);
	float& GetRoughness();

	void SetMetallic(float fMetallic);
	float& GetMetallic();

	void SetAlbedo(const glm::vec3& vec3Albedo);
	glm::vec3& GetAlbedo();
private:
	float m_fRoughness;
	float m_fMetallic;
	glm::vec3 m_vec3Albedo;
};
SAND_TABLE_NAMESPACE_END

