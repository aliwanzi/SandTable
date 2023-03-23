#include "pch.h"
#include "NoiseColorTexture.h"
SAND_TABLE_NAMESPACE_BEGIN

NoiseColorTexture::NoiseColorTexture() :
	m_spPerlinNoise(CreateRef<PerlinNoise>())
{
}


glm::dvec3 NoiseColorTexture::GetColor(const glm::dvec3& SamplePoint, glm::dvec2 UV) const
{
	return glm::dvec3(1.0) * 0.5 * (1 + glm::sin(4 * SamplePoint.z + 10 * m_spPerlinNoise->GetNoise(SamplePoint)));
}

SAND_TABLE_NAMESPACE_END
