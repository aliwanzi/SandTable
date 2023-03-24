#include "pch.h"
#include "NoiseColorTexture.h"
SAND_TABLE_NAMESPACE_BEGIN

NoiseColorTexture::NoiseColorTexture() :
	m_spPerlinNoise(CreateRef<PerlinNoise>()),
	m_dNoiseScale(0.0),
	m_iTurbDepth(0)
{
}



void NoiseColorTexture::SetTurbDepth(int iTurbDepth)
{
	m_iTurbDepth = iTurbDepth;
}

void NoiseColorTexture::SetNoiseScale(double dScale)
{
	m_dNoiseScale = dScale;
}

glm::dvec3 NoiseColorTexture::GetColor(const glm::dvec3& SamplePoint, const glm::dvec2& UV) const
{
	return glm::dvec3(1.0) * 0.5 * (1.0 +
		sin(m_dNoiseScale * SamplePoint.z + 10 * m_spPerlinNoise->Turb(m_dNoiseScale * SamplePoint)));
}

SAND_TABLE_NAMESPACE_END
