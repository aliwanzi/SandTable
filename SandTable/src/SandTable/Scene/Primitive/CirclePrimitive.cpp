#include "pch.h"
#include "CirclePrimitive.h"

SAND_TABLE_NAMESPACE_BEGIN
CirclePrimitive::CirclePrimitive() :
	m_vec4Color(glm::vec4(1.f)),
	m_fRadius(0.5f),
	m_fThickness(1.f),
	m_fFade(0.005f)
{
}

void CirclePrimitive::SetPosition(const std::array<glm::vec4, 4>& arrayPosition)
{
	m_arrayPosition = arrayPosition;
}

const std::array<glm::vec4, 4>& CirclePrimitive::GetPosition() const
{
	return m_arrayPosition;
}

void CirclePrimitive::SetColor(const glm::vec4& vec4Color)
{
	m_vec4Color = vec4Color;
}

const glm::vec4& CirclePrimitive::GetColor() const
{
	return m_vec4Color;
}

glm::vec4& CirclePrimitive::GetColor()
{
	return m_vec4Color;
}

void CirclePrimitive::SetRadius(float fRadius)
{
	m_fRadius = fRadius;
}

float CirclePrimitive::GetRadius() const
{
	return m_fRadius;
}

float& CirclePrimitive::GetRadius()
{
	return m_fRadius;
}

void CirclePrimitive::SetThickness(float fThickness)
{
	m_fThickness = fThickness;
}

float CirclePrimitive::GetThickness() const
{
	return m_fThickness;
}

float& CirclePrimitive::GetThickness()
{
	return m_fThickness;
}

void CirclePrimitive::SetFade(float fFade)
{
	m_fFade = fFade;
}

float CirclePrimitive::GetFade() const
{
	return m_fFade;
}

float& CirclePrimitive::GetFade()
{
	return m_fFade;
}

SAND_TABLE_NAMESPACE_END
