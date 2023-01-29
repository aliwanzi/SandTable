#include "pch.h"
#include "ParticleSystem2D.h"
#include "SandTable/System/Random.h"
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

	for (auto& particle : m_vecParticlePool)
	{
		if (!particle.Active)
			continue;
		// Fade away particles
		float life = particle.LifeRemaining / particle.LifeTime;
		float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);
		switch (particle.ParticleType)
		{
			case ParticleRenderType::COLOR:
			{
				glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
				Render2D::DrawQuad(glm::vec3(particle.Position.x, particle.Position.y, 0.2f), particle.Rotation, glm::vec2(size), color);
				break;
			}
			case ParticleRenderType::TEXTURE:
			{
				SAND_TABLE_ASSERT(particle.RefTexture, "Texture is null in render particle");
				Render2D::DrawQuad(glm::vec3(particle.Position.x, particle.Position.y, 0.2f), particle.Rotation, glm::vec2(size), particle.RefTexture);
				break;
			}
			default:
				break;
		}
		
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

	particle.ParticleType = particleProps.ParticleType;
	particle.RefTexture = particleProps.RefTexture;

	m_vecParticlePool.emplace_back(particle);
}

SAND_TABLE_NAMESPACE_END