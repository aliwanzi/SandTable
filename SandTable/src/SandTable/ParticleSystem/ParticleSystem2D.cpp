#include "pch.h"
#include "ParticleSystem2D.h"
#include "SandTable/ParticleSystem/Random.h"
#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>
#include "SandTable/Render/Render2D.h"

SAND_TABLE_NAMESPACE_BEGIN

ParticleSystem2D::ParticleSystem2D()
{
	m_vecParticlePool.reserve(1000);
}

void ParticleSystem2D::OnUpdate(TimeStep timeStep)
{
	for (auto& particle : m_vecParticlePool)
	{
		if (!particle.Active)
			continue;

		if (particle.LifeRemaining <= 0.0f)
		{
			particle.Active = false;
			continue;
		}

		particle.LifeRemaining -= timeStep;
		particle.Position += particle.Velocity * (float)timeStep;
		particle.Rotation += 0.01f * timeStep;
	}
}

void ParticleSystem2D::OnRender(const Ref<OrthoGraphicCamera>& spOrthoGraphicCamera)
{
	Render2D::BeginScene(spOrthoGraphicCamera);

	bool bDraw(false);
	for (auto& particle : m_vecParticlePool)
	{
		if (!particle.Active)
			continue;
		bDraw = true;
		// Fade away particles
		float life = particle.LifeRemaining / particle.LifeTime;
		glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);

		float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);
		Render2D::DrawQuad(glm::vec3(particle.Position.x, particle.Position.y, 0.2f), particle.Rotation, glm::vec2(size), particle.RefTexture);
	}
	Render2D::EndScene();
}

void ParticleSystem2D::Emit(const ParticleProps& particleProps)
{
	Particle particle;
	particle.Active = true;
	particle.Position = particleProps.Position;
	particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

	// Velocity
	particle.Velocity = particleProps.Velocity;
	particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
	particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

	// Color
	particle.ColorBegin = particleProps.ColorBegin;
	particle.ColorEnd = particleProps.ColorEnd;

	particle.LifeTime = particleProps.LifeTime;
	particle.LifeRemaining = particleProps.LifeTime;
	particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
	particle.SizeEnd = particleProps.SizeEnd;

	particle.RefTexture = particleProps.RefTexture;

	m_vecParticlePool.emplace_back(particle);
}

SAND_TABLE_NAMESPACE_END