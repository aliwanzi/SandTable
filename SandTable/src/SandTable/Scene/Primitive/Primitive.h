#pragma once
#include "SandTable/Render/Buffer/VertexBufferLayout.h"

SAND_TABLE_NAMESPACE_BEGIN

class Primitive
{
public:
	virtual ~Primitive() = default;
	Primitive();

	void SetPosition(const std::vector<glm::vec4>& vecPosition);
	void AddPosition(const glm::vec4& vec4Position);
	const std::vector<glm::vec4>& GetPositions()const;

	void SetColor(const glm::vec4& vec4Color);
	const glm::vec4& GetColor()const;
	glm::vec4& GetColor();

	const std::vector<VertexBufferElement>& GetVertexBufferElement()const;

private:
	std::vector<glm::vec4> m_vecPosition;
	glm::vec4 m_vec4Color;
	std::vector<VertexBufferElement> m_vecVertexBufferElement;
};

SAND_TABLE_NAMESPACE_END

