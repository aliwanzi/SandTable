#pragma once
#include "SandTable/Render/Camera/OrthoGraphicCameraController.h"
#include "SandTable/Render/Texture/SubTexture2D.h"

SAND_TABLE_NAMESPACE_BEGIN
class VertexArray;
class Buffer;
class Shader;
struct SpriteRenderComponent;

class Render2D
{
public:
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;

		float EntityID;
	};

	//Performance Test
	struct Statistics
	{
		unsigned int DrawCalls;
		unsigned int QuadCount;
		unsigned int GetTotalVertexCount() { return QuadCount * 4; }
		unsigned int GetTotalIndexCount() { return QuadCount * 6; }
	};

	struct Render2DStroge
	{
		unsigned int QuadIndexCount = 0;
		const unsigned int MaxQuads = 10000;
		const unsigned int MaxVertices = MaxQuads * 4;
		const unsigned int MaxIndices = MaxQuads * 6;

		Ref<VertexArray> VertexArray;

		std::vector<Vertex> Vertex;
		std::array<glm::vec4, 4> VertexPosition;
		Ref<Buffer> VertexBuffer;

		Ref<Shader> Shader;

		std::unordered_map<unsigned int, Ref<Texture>> TextureSlots;
		Ref<Texture> WhiteTexture;
		std::array<glm::vec2, 4> TextureCoord;

		Statistics Stats;
	};

	static void Init();
	static void ShutDown();
	static void BeginScene(const Ref<Camera>& spCamera);
	static void EndScene();
	static void Flush();

	static void DrawSprite(const glm::mat4& mat4Transform, const SpriteRenderComponent& spriteRenderComponent, int iEntityID);

	static void DrawQuad(const glm::vec2& vec2Position, float fRotation, const glm::vec2& vec2Size, const glm::vec4& vec4Color);
	static void DrawQuad(const glm::vec3& vec3Position, float fRotation, const glm::vec2& vec2Size, const glm::vec4& vec4Color);

	static void DrawQuad(const glm::vec2& vec2Position, float fRotation, const glm::vec2& vec2Size, const Ref<Texture>& spTexture, 
		float fFactor = 1.0f, const glm::vec4& vec4Color = glm::vec4(1.0f));
	static void DrawQuad(const glm::vec3& vec3Position, float fRotation, const glm::vec2& vec2Size, const Ref<Texture>& spTexture, 
		float fFactor = 1.0f, const glm::vec4& vec4Color = glm::vec4(1.0f));

	static void DrawQuad(const glm::vec2& vec2Position, float fRotation, const glm::vec2& vec2Size, const Ref<SubTexture2D>& spSubTexture2D,
		float fFactor = 1.0f, const glm::vec4& vec4Color = glm::vec4(1.0f));
	static void DrawQuad(const glm::vec3& vec3Position, float fRotation, const glm::vec2& vec2Size, const Ref<SubTexture2D>& spSubTexture2D,
		float fFactor = 1.0f, const glm::vec4& vec4Color = glm::vec4(1.0f));

	static void DrawQuad(const glm::mat4& mat4Transform, const glm::vec4& vec4Color, int iEntityID = -1);
	static void DrawQuad(const glm::mat4& mat4Transform, const Ref<Texture>& spTexture,
		float fFactor = 1.0f, const glm::vec4& vec4Color = glm::vec4(1.0f));

	static void ResetStats();
	static Statistics GetStats();
private:
	static void StartBatch();
	static void NextBatch();
	static Ref<Render2DStroge> m_spRender2DStroge;
};


SAND_TABLE_NAMESPACE_END

