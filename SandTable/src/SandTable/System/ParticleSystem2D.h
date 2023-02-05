#pragma once
SAND_TABLE_NAMESPACE_BEGIN
class Texture;
class Camera;
class QuadPrimitive;
enum class ParticleRenderType
{
	COLOR = 0,
	TEXTURE
};

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
	Ref<Texture> RefTexture;
	ParticleRenderType ParticleType;
};

class ParticleSystem2D
{
public:
	ParticleSystem2D();

	void OnUpdate(TimeStep timeStep);
	void OnRender(const Ref<Camera>& spCamera);

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

		ParticleRenderType ParticleType;
		Ref<Texture> RefTexture;

		bool Active;
	};

	std::vector<Particle> m_vecParticlePool;
	Ref<QuadPrimitive> m_spQuadPrimtive;
};

SAND_TABLE_NAMESPACE_END
