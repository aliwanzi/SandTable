#pragma once

SAND_TABLE_NAMESPACE_BEGIN
class ColorTexture
{
public:
	virtual glm::dvec3 GetColor(const glm::dvec3& SamplePoint, glm::dvec2 UV) const = 0;
};
SAND_TABLE_NAMESPACE_END

