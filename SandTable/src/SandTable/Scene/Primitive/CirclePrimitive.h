#pragma once
#include "Primitive.h"

SAND_TABLE_NAMESPACE_BEGIN

const std::vector<VertexBufferElement> vecCircleBufferElement
{
	{ VertexDataType::Float3 },
	{ VertexDataType::Float3 },
	{ VertexDataType::Float4 },
	{ VertexDataType::Float },
	{ VertexDataType::Float },
	{ VertexDataType::Float }
};


class CirclePrimitive :public Primitive
{
public:
	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;

		float EntityID;
	};

	CirclePrimitive();

	void SetPosition(const std::array<glm::vec4, 4>& arrayPosition);
	const std::array<glm::vec4, 4>& GetPosition()const;

	void SetColor(const glm::vec4& vec4Color);
	const glm::vec4& GetColor()const;
	glm::vec4& GetColor();

	void SetRadius(float fRadius);
	float GetRadius()const;
	float& GetRadius();

	void SetThickness(float fThickness);
	float GetThickness()const;
	float& GetThickness();

	void SetFade(float fFade);
	float GetFade()const;
	float& GetFade();
private:
	std::array<glm::vec4, 4> m_arrayPosition =
	{
		glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
		glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),
		glm::vec4(0.5f,  0.5f, 0.0f, 1.0f),
		glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f)
	};
	glm::vec4 m_vec4Color;
	float m_fRadius;
	float m_fThickness;
	float m_fFade;
};

SAND_TABLE_NAMESPACE_END