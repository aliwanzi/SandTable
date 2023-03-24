#pragma once
#include "ColorTexture.h"
SAND_TABLE_NAMESPACE_BEGIN

class CheckerColorTexture :public ColorTexture
{
public:
	CheckerColorTexture(Ref<ColorTexture> spEven, Ref<ColorTexture> spOdd, int iCheckSize = 10);
	// Í¨¹ý Color ¼Ì³Ð
	virtual glm::dvec3 GetColor(const glm::dvec3& SamplePoint, const glm::dvec2& UV) const override;
private:
	Ref<ColorTexture> m_spEven;
	Ref<ColorTexture> m_spOdd;
	int m_iCheckSize;
};

SAND_TABLE_NAMESPACE_END

