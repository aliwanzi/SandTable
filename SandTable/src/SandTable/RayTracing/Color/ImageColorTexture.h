#pragma once
#include "ColorTexture.h"
SAND_TABLE_NAMESPACE_BEGIN

class ImageColorTexture :public ColorTexture
{
public:
	// Í¨¹ý ColorTexture ¼Ì³Ð
	virtual glm::dvec3 GetColor(const glm::dvec3& SamplePoint, glm::dvec2 UV) const override;
};

SAND_TABLE_NAMESPACE_END

