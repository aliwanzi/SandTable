#pragma once
#include "ColorTexture.h"
#include "PerlinNoise.h"

SAND_TABLE_NAMESPACE_BEGIN
class NoiseColorTexture :public ColorTexture
{
public:
	NoiseColorTexture();
	void SetTurbDepth(int iTurbDepth);
	void SetNoiseScale(double dScale);
	// Í¨¹ý Color ¼Ì³Ð
	virtual glm::dvec3 GetColor(const glm::dvec3& SamplePoint, const glm::dvec2& UV) const override;
private:
	int m_iTurbDepth;
	double m_dNoiseScale;
	Ref<PerlinNoise> m_spPerlinNoise;
};
SAND_TABLE_NAMESPACE_END

