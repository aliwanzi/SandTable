#include "pch.h"
#include "Primitive.h"

SAND_TABLE_NAMESPACE_BEGIN
Primitive::Primitive():
	m_vec4Color(glm::vec4(1.f))
{
	m_vecPosition.reserve(4);
}

void Primitive::SetPosition(const std::vector<glm::vec4>& vecPosition)
{
	m_vecPosition = vecPosition;
}

void Primitive::AddPosition(const glm::vec4& vec4Position)
{
	m_vecPosition.push_back(vec4Position);
}

const std::vector<glm::vec4>& Primitive::GetPositions() const
{
	return m_vecPosition;
}

void Primitive::SetColor(const glm::vec4& vec4Color)
{
	m_vec4Color = vec4Color;
}

const glm::vec4& Primitive::GetColor() const
{
	return m_vec4Color;
}

glm::vec4& Primitive::GetColor()
{
	return m_vec4Color;
}

const std::vector<VertexBufferElement>& Primitive::GetVertexBufferElement() const
{
	return m_vecVertexBufferElement;
}

SAND_TABLE_NAMESPACE_END