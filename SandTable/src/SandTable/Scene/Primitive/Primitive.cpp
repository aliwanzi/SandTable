#include "pch.h"
#include "Primitive.h"

SAND_TABLE_NAMESPACE_BEGIN
Primitive::Primitive() :
	m_iEntityID(-1)
{
}

void Primitive::SetEntityID(int iEntityID)
{
	m_iEntityID = iEntityID;
}

int Primitive::GetEntityID() const
{
	return m_iEntityID;
}

const std::vector<VertexBufferElement>& Primitive::GetVertexBufferElement() const
{
	return m_vecVertexBufferElement;
}

SAND_TABLE_NAMESPACE_END