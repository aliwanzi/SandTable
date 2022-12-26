#pragma once
#include "SandTable/Core/Core.h"
#include "SandTable/Core/TimeStep.h"
#include "SandTable/Render/Camera/OrthoGraphicCamera.h"
#include "SandTable/Render/Shader.h"
#include <glm/glm.hpp>

SAND_TABLE_NAMESPACE_BEGIN

struct ParticleProps
{
	glm::vec2 Position;
	glm::vec2 Velocity;
	glm::vec2 VelocityVariation;
	glm::vec4 ColorBegin;
	glm::vec4 ColorEnd;
	float SizeBegin;
	float SizeEnd;
	float SizeVariation;
	float LifeTime;
};

class ParticleSystem2D
{
public:
	ParticleSystem2D();

	void OnUpdate(TimeStep timeStep);
	void OnRender(const Ref<OrthoGraphicCamera>& spOrthoGraphicCamera);

	void Emit(const ParticleProps& particleProps);
private:
	struct Particle
	{
		glm::vec2 Position;
		glm::vec2 Velocity;
		glm::vec4 ColorBegin;
		glm::vec4 ColorEnd;
		float Rotation;
		float SizeBegin;
		float SizeEnd;

		float LifeTime;
		float LifeRemaining;

		bool Active;
	};

	std::vector<Particle> m_vecParticlePool;
};

SAND_TABLE_NAMESPACE_END
