#include "pch.h"
#include "CheckerColorTexture.h"
SAND_TABLE_NAMESPACE_BEGIN

CheckerColorTexture::CheckerColorTexture(Ref<ColorTexture> spEven, Ref<ColorTexture> spOdd, int iCheckSize):
    m_spEven(spEven),m_spOdd(spOdd),m_iCheckSize(iCheckSize)
{
}


glm::dvec3 CheckerColorTexture::GetColor(const glm::dvec3& SamplePoint, const glm::dvec2& UV) const
{
    auto sine = glm::sin(SamplePoint.x * m_iCheckSize) * glm::sin(SamplePoint.y * m_iCheckSize) * glm::sin(SamplePoint.z * m_iCheckSize);
	return sine < 0 ? m_spOdd->GetColor(SamplePoint, UV) : m_spEven->GetColor(SamplePoint, UV);
}
SAND_TABLE_NAMESPACE_END