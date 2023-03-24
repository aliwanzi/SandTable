#include "pch.h"
#include "SolidColorTexture.h"
SAND_TABLE_NAMESPACE_BEGIN

SolidColorTexture::SolidColorTexture(const glm::dvec3& color):
    m_vec3Color(color)
{
}

glm::dvec3 SolidColorTexture::GetColor(const glm::dvec3& SamplePoint, const glm::dvec2& UV) const
{
    return m_vec3Color;
}

SAND_TABLE_NAMESPACE_END