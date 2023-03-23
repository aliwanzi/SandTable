#include "pch.h"
#include "PerlinNoise.h"
#include "SandTable/Math/Random.h"

SAND_TABLE_NAMESPACE_BEGIN
PerlinNoise::PerlinNoise(uint32_t uiPointCount):
	m_uiPointCount(uiPointCount),
	m_spPermBuffer(CreateRef<DataBuffer>(uiPointCount,sizeof(glm::ivec3)/sizeof(uint8_t)))
{
	m_vecNoise.resize(uiPointCount);
	auto permBuffer = m_spPermBuffer->As<glm::ivec3>();
	for (uint32_t i = 0; i < uiPointCount; i++)
	{
		permBuffer[i].x = permBuffer[i].y = permBuffer[i].z = i;
		m_vecNoise[i] = Random::UnitSphere();
	}

	for (uint32_t i = uiPointCount - 1; i > 0; i--)
	{
		int targetX = Random::Uint(0, i);
		int targetY = Random::Uint(0, i);
		int targetZ = Random::Uint(0, i);
		std::swap(permBuffer[i].x, permBuffer[targetX].x);
		std::swap(permBuffer[i].y, permBuffer[targetY].y);
		std::swap(permBuffer[i].z, permBuffer[targetZ].z);
	}
}

double PerlinNoise::GetNoise(const glm::dvec3& SamplePoint) const
{
	auto uvw = SamplePoint - glm::floor(SamplePoint);

	auto i = static_cast<int>(glm::floor(SamplePoint.x));
	auto j = static_cast<int>(glm::floor(SamplePoint.y));
	auto k = static_cast<int>(glm::floor(SamplePoint.z));

	auto permBuffer = m_spPermBuffer->As<glm::ivec3>();

	glm::dvec3 noise[2][2][2];
	for (int di = 0; di < 2; di++)
	{
		for (int dj = 0; dj < 2; dj++)
		{
			for (int dk = 0; dk < 2; dk++)
			{
				noise[di][dj][dk] = m_vecNoise[
					permBuffer[(i + di) & 255].x ^
						permBuffer[(j + dj) & 255].y ^
						permBuffer[(k + dk) & 255].z
				];
			}
		}
	}

	return TrilinearInterp(noise, uvw);
}

double PerlinNoise::Turb(const glm::dvec3& SamplePoint, int iDepth) const
{
	double value(0.0), weight(1.0);
	glm::dvec3 tempPoint(SamplePoint);
	for (int i = 0; i < iDepth; i++)
	{
		value += weight * GetNoise(tempPoint);
		weight *= 0.5;
		tempPoint *= 2;
	}
	return value;
}

double PerlinNoise::TrilinearInterp(const glm::dvec3 noise[2][2][2], const glm::dvec3& uvw) const
{
	glm::dvec3 base = uvw * uvw * (glm::dvec3(3.0) - 2.0 * uvw);

	double value = 0.0;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++) 
			{
				glm::dvec3 weight(uvw - glm::dvec3(i, j, k));
				glm::dvec3 accum = glm::dvec3(i, j, k) * base + (glm::dvec3(1) - glm::dvec3(i, j, k)) * (glm::dvec3(1) - base);
				value += accum.x * accum.y * accum.z * glm::dot(noise[i][j][k], weight);
			}
		}
	}


	return value;
}

double PerlinNoise::TrilinearInterp(const double noise[2][2][2], const glm::dvec3& uvw) const
{
	glm::dvec3 base = uvw * uvw * (glm::dvec3(3.0) - 2.0 * uvw);
	double value = 0.0;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				glm::dvec3 accum = glm::dvec3(i, j, k) * base + (glm::dvec3(1) - glm::dvec3(i, j, k)) * (glm::dvec3(1) - base);
				value += accum.x * accum.y * accum.z * noise[i][j][k];
			}
		}
	}


	return value;
}

SAND_TABLE_NAMESPACE_END


