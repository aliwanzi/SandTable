#include "pch.h"
#include "QuadPrimitive.h"

SAND_TABLE_NAMESPACE_BEGIN

QuadPrimitive::QuadPrimitive() :
	m_vec4Color(glm::vec4(1.f)),
	m_iTexIndex(-1),
	m_fTilingFactor(1.f)
{
}

void QuadPrimitive::SetPosition(const std::array<glm::vec4, 4>& arrayPosition)
{
	m_arrayPosition = arrayPosition;
}

const std::array<glm::vec4, 4>& QuadPrimitive::GetPosition() const
{
	return m_arrayPosition;
}

void QuadPrimitive::SetColor(const glm::vec4& vec4Color)
{
	m_vec4Color = vec4Color;
}

const glm::vec4& QuadPrimitive::GetColor() const
{
	return m_vec4Color;
}

glm::vec4& QuadPrimitive::GetColor()
{
	return m_vec4Color;
}

void QuadPrimitive::SetTexCoord(const std::array<glm::vec2, 4>& arrayTexCoord)
{
	m_arrayTexCoord = arrayTexCoord;
}

const std::array<glm::vec2, 4>& QuadPrimitive::GetTexCoord() const
{
	return m_arrayTexCoord;
}

void QuadPrimitive::SetTexIndex(int iTexIndex)
{
	m_iTexIndex = iTexIndex;
}

int QuadPrimitive::GetTexIndex() const
{
	return m_iTexIndex;
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
