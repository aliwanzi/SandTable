#pragma once
//#include "SandTable/Render/Camera/OrthoGraphicCameraController.h"
//#include "SandTable/Render/Texture/SubTexture2D.h"
#include "SandTable/Scene/Components.h"

SAND_TABLE_NAMESPACE_BEGIN
class VertexArray;
class Buffer;
class Shader;
class Texture;
class QuadDrawStatics;

class Render2D
{
public:
	struct Render2DStroge
	{
		Ref<QuadDrawStatics> spQuadDrawStatics;
		Ref<VertexArray> spQuadVertexArray;
		Ref<Buffer> spQuadVertexBuffer;
		Ref<Shader> spQuadShader;
		Ref<Texture> spWhiteTexture;
		std::vector<QuadPrimitive::QuadVertex> vecQuadVertexs;
		
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
	static void DrawPrimitive(const glm::vec3& vec3Pan, float fRotation, const glm::vec3& vec3Scale, Ref<QuadPrimitive> spPrimitive, Ref<Texture> spTexture = nullptr, int iEntityID = -1);
	static void DrawPrimitive(const glm::mat4& mat4Transform, Ref<QuadPrimitive> spPrimitive, Ref<Texture> spTexture = nullptr, int iEntityID = -1);

	static void ResetStats();
	static const Ref<QuadDrawStatics>& GetQuadStatic();
private:
	static void StartBatch();
	static void NextBatch();
	static Ref<Render2DStroge> m_spRender2DStroge;
};


SAND_TABLE_NAMESPACE_END

