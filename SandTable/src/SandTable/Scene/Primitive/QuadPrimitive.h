#pragma once
#include "Primitive.h"

SAND_TABLE_NAMESPACE_BEGIN

const std::vector<VertexBufferElement> vecQuadBufferElement
{
	{ VertexDataType::Float3 },
	{ VertexDataType::Float4 },
	{ VertexDataType::Float2 },
	{ VertexDataType::Float },
	{ VertexDataType::Float },
	{ VertexDataType::Float }
};

class QuadPrimitive :public Primitive
{
public:
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;

		float EntityID;
	};

	QuadPrimitive();

	void SetPosition(const std::array<glm::vec4, 4>& arrayPosition);
	const std::array<glm::vec4, 4>& GetPosition()const;

	void SetColor(const glm::vec4& vec4Color);
	const glm::vec4& GetColor()const;
	glm::vec4& GetColor();

	void SetTexCoord(const std::array<glm::vec2, 4>& arrayTexCoord);
	const std::array<glm::vec2, 4>& GetTexCoord()const;

	void SetTexIndex(int iTexIndex);
	int GetTexIndex()const;

	void SetTilingFactor(float fTilingFactor);
	float GetTilingFactor()const;
	float& GetTilingFactor();

private:
	std::array<glm::vec4, 4> m_arrayPosition =
	{
		glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
		glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),
		glm::vec4(0.5f,  0.5f, 0.0f, 1.0f),
		glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f)
	};
	std::array<glm::vec2, 4> m_arrayTexCoord =
	{
		glm::vec2(0.f, 0.f),
		glm::vec2(1.f, 0.f),
		glm::vec2(1.f, 1.f),
		glm::vec2(0.f, 1.f)
	};
	glm::vec4 m_vec4Color;
	int m_iTexIndex;
	float m_fTilingFactor;
};
SAND_TABLE_NAMESPACE_END