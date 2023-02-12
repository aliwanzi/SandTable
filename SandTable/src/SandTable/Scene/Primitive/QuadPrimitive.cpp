#include "pch.h"
#include "QuadPrimitive.h"

SAND_TABLE_NAMESPACE_BEGIN

QuadPrimitive::QuadPrimitive() :
	m_fTilingFactor(1.f)
{
	AddPosition(glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f));
	AddPosition(glm::vec4(0.5f, -0.5f, 0.0f, 1.0f));
	AddPosition(glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));
	AddPosition(glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f));
}

void QuadPrimitive::SetTexCoord(const std::array<glm::vec2, 4>& arrayTexCoord)
{
	m_arrayTexCoord = arrayTexCoord;
}

const std::array<glm::vec2, 4>& QuadPrimitive::GetTexCoord() const
{
	return m_arrayTexCoord;
}

void QuadPrimitive::SetTilingFactor(float fTilingFactor)
{
	m_fTilingFactor = fTilingFactor;
}

float QuadPrimitive::GetTilingFactor() const
{
	return m_fTilingFactor;
}

float& QuadPrimitive::GetTilingFactor()
{
	return m_fTilingFactor;
}

SAND_TABLE_NAMESPACE_END
