#pragma once
#include "ColorTexture.h"
#include "PerlinNoise.h"

SAND_TABLE_NAMESPACE_BEGIN
class NoiseColorTexture :public ColorTexture
{
public:
	NoiseColorTexture();
	// Í¨¹ý Color ¼Ì³Ð
	virtual glm::dvec3 GetColor(const glm::dvec3& SamplePoint, glm::dvec2 UV) const override;
private:
	Ref<PerlinNoise> m_spPerlinNoise;
};
SAND_TABLE_NAMESPACE_END

