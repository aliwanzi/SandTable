#pragma once
#include "Primitive.h"

SAND_TABLE_NAMESPACE_BEGIN

const std::vector<VertexBufferElement> vecCircleBufferElement
{
	{ VertexDataType::Float3 },
	{ VertexDataType::Float3 },
	{ VertexDataType::Float4 },
	{ VertexDataType::Float2 },
	{ VertexDataType::Float },
	{ VertexDataType::Float },
	{ VertexDataType::Float },
	{ VertexDataType::Float }
};


class CirclePrimitive :public Primitive
{
public:
	struct CircleVertex
	{
		glm::vec3	WorldPosition;
		glm::vec3	LocalPosition;
		glm::vec4	Color;
		glm::vec2	TexCoord;
		float		TexIndex;
		float		Thickness;
		float		Fade;

		float EntityID;
	};

	CirclePrimitive();

	void SetRadius(float fRadius);
	float GetRadius()const;
	float& GetRadius();

	void SetTexCoord(const std::array<glm::vec2, 4>& arrayTexCoord);
	const std::array<glm::vec2, 4>& GetTexCoord()const;

	void SetThickness(float fThickness);
	float GetThickness()const;
	float& GetThickness();

	void SetFade(float fFade);
	float GetFade()const;
	float& GetFade();
private:
	std::array<glm::vec2, 4> m_arrayTexCoord =
	{
		glm::vec2(0.f, 0.f),
		glm::vec2(1.f, 0.f),
		glm::vec2(1.f, 1.f),
		glm::vec2(0.f, 1.f)
	};

	glm::vec4 m_vec4Color;
	float m_fRadius;
	float m_fThickness;
	float m_fFade;
};

SAND_TABLE_NAMESPACE_END