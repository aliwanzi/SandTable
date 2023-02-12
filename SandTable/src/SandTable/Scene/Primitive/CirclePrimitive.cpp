#include "pch.h"
#include "CirclePrimitive.h"

SAND_TABLE_NAMESPACE_BEGIN
CirclePrimitive::CirclePrimitive() :
	m_vec4Color(glm::vec4(1.f)),
	m_fRadius(0.5f),
	m_fThickness(1.f),
	m_fFade(0.005f)
{
	AddPosition(glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f));
	AddPosition(glm::vec4(0.5f, -0.5f, 0.0f, 1.0f));
	AddPosition(glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));
	AddPosition(glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f));
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

void CirclePrimitive::SetTexCoord(const std::array<glm::vec2, 4>& arrayTexCoord)
{
	m_arrayTexCoord = arrayTexCoord;
}

const std::array<glm::vec2, 4>& CirclePrimitive::GetTexCoord() const
{
	return m_arrayTexCoord;
}

SAND_TABLE_NAMESPACE_END
