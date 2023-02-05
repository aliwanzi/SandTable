#pragma once
#include "Primitive.h"

SAND_TABLE_NAMESPACE_BEGIN

class CirclePrimitive :public Primitive
{
public:
	struct CircleVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		float Thickness;
		float Fade;

		float EntityID;
	};

	CirclePrimitive();

	void SetPosition(const glm::vec3& vec3Position);
	const glm::vec3& GetPosition() const;

	void SetColor(const glm::vec4& vec4Color);
	const glm::vec4& GetColor()const;

	void SetRadius(float fRadius);
	float GetRadius()const;

	void SetThickness(float fThickness);
	float GetThickness()const;

	void SetFade(float fFade);
	float GetFade()const;
private:
	glm::vec3 m_vec3Position;
	glm::vec4 m_vec4Color;
	float m_fRadius;
	float m_fThickness;
	float m_fFade;
};

SAND_TABLE_NAMESPACE_END