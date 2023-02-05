#include "pch.h"
#include "CirclePrimitive.h"

SAND_TABLE_NAMESPACE_BEGIN
CirclePrimitive::CirclePrimitive() :
	m_vec3Position(0.f),
	m_vec4Color(glm::vec4(0.f)),
	m_fRadius(0.f),
	m_fThickness(0.f),
	m_fFade(0.f)
{
}

void CirclePrimitive::SetPosition(const glm::vec3& vec3Position)
{
	m_vec3Position = vec3Position;
}

const glm::vec3& CirclePrimitive::GetPosition() const
{
	return m_vec3Position;
}

void CirclePrimitive::SetColor(const glm::vec4& vec4Color)
{
	m_vec4Color = vec4Color;
}

const glm::vec4& CirclePrimitive::GetColor() const
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

void CirclePrimitive::SetThickness(float fThickness)
{
	m_fThickness = fThickness;
}

float CirclePrimitive::GetThickness() const
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

SAND_TABLE_NAMESPACE_END
