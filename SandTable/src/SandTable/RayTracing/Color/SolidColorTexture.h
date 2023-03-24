#pragma once
#include "ColorTexture.h"

SAND_TABLE_NAMESPACE_BEGIN

class SolidColorTexture:public ColorTexture
{
public:
	SolidColorTexture(const glm::dvec3& color);
	// Í¨¹ý Color ¼Ì³Ð
	virtual glm::dvec3 GetColor(const glm::dvec3& SamplePoint, const glm::dvec2& UV) const override;
private:
	glm::dvec3 m_vec3Color;
};

SAND_TABLE_NAMESPACE_END
