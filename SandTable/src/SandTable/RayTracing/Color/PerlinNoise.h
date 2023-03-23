#pragma once
#include "SandTable/Core/DataBuffer.h"
SAND_TABLE_NAMESPACE_BEGIN
class PerlinNoise
{
public:
	PerlinNoise(uint32_t uiPointCount = 256);
	double GetNoise(const glm::dvec3& SamplePoint)const;
private:
	double TrilinearInterp(const glm::dvec3 noise[2][2][2], const glm::dvec3& uvw) const;
private:
	uint32_t m_uiPointCount;
	Ref<DataBuffer> m_spPermBuffer;
	std::vector<glm::dvec3> m_vecNoise;
};

SAND_TABLE_NAMESPACE_END

