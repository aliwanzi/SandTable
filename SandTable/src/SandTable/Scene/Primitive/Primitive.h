#pragma once
#include "SandTable/Render/Buffer/VertexBufferLayout.h"

SAND_TABLE_NAMESPACE_BEGIN

class Primitive
{
public:
	virtual ~Primitive() = default;
	Primitive();
	void SetEntityID(int iEntityID);
	int GetEntityID()const;
	const std::vector<VertexBufferElement>& GetVertexBufferElement()const;
protected:
	int m_iEntityID;
	std::vector<VertexBufferElement> m_vecVertexBufferElement;
};

SAND_TABLE_NAMESPACE_END

