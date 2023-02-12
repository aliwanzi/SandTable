#pragma once
#include "SandTable/Scene/Components.h"

SAND_TABLE_NAMESPACE_BEGIN
class VertexArray;
class Buffer;
class Shader;
class Texture;
class DrawStatistics;

class Render2D
{
public:
	struct Render2DStroge
	{
		Ref<DrawStatistics> spQuadDrawStatics;
		Ref<VertexArray> spQuadVertexArray;
		Ref<Buffer> spQuadVertexBuffer;
		Ref<Shader> spQuadShader;
		Ref<Texture> spWhiteTexture;
		std::vector<QuadPrimitive::QuadVertex> vecQuadVertexs;

		Ref<DrawStatistics> spCircleDrawStatics;
		Ref<VertexArray> spCircleVertexArray;
		Ref<Buffer> spCircleVertexBuffer;
		Ref<Shader> spCircleShader;
		std::vector<CirclePrimitive::CircleVertex> vecCircleVertexs;

		Ref<DrawStatistics> spLineDrawStatics;
		Ref<VertexArray> spLineVertexArray;
		Ref<Buffer> spLineVertexBuffer;
		Ref<Shader> spLineShader;
		std::vector<LinePrimitive::LineVertex> vecLineVertexs;

		std::unordered_map<unsigned int, Ref<Texture>> TextureSlots;

		CameraMatrix CameraBuffer;
		Ref<Buffer> CameraUniformBuffer;
	};

	static void Init();
	static void ShutDown();
	static void BeginScene(const Ref<Camera>& spCamera);
	static void EndScene();
	static void Flush();

	static void DrawSprite(const glm::mat4& mat4Transform, const SpriteRenderComponent& spriteRenderComponent, int iEntityID);
	static void DrawCircle(const glm::mat4& mat4Transform, const CircleRenderComponent& circleRenderComponent, int iEntityID);
	static void DrawPrimitive(const glm::vec3& vec3Pan, float fRotation, const glm::vec3& vec3Scale, Ref<Primitive> spPrimitive, Ref<Texture> spTexture = nullptr, int iEntityID = -1);
	static void DrawPrimitive(const glm::mat4& mat4Transform, Ref<Primitive> spPrimitive, Ref<Texture> spTexture = nullptr, int iEntityID = -1);
	static void DrawBoundary(const glm::mat4& mat4Transform, Ref<Primitive> spPrimitive);

	static void ResetStats();

	static const Ref<DrawStatistics>& GetQuadStatic();
	static const Ref<DrawStatistics>& GetCircleStatic();

private:
	static void CreateRenderPara(Ref<DrawStatistics> spDrawStatistics, Ref<VertexArray>& spVertexArray, Ref<Buffer>& spVertexBuffer, const std::vector<VertexBufferElement>& vertexBufferElement);
	static void DrawPrimitive(const glm::mat4& mat4Transform, Ref<QuadPrimitive> spPrimitive, Ref<Texture> spTexture, int iEntityID);
	static void DrawPrimitive(const glm::mat4& mat4Transform, Ref<CirclePrimitive> spPrimitive, Ref<Texture> spTexture, int iEntityID);
	static void DrawPrimitive(const glm::mat4& mat4Transform, Ref<LinePrimitive> spPrimitive, Ref<Texture> spTexture, int iEntityID);
	static void StartBatch();
	static void NextBatch();
	static Ref<Render2DStroge> m_spRender2DStroge;
};


SAND_TABLE_NAMESPACE_END

