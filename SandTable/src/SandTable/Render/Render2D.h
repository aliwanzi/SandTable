#pragma once
#include "SandTable/Render/Camera/OrthoGraphicCameraController.h"

SAND_TABLE_NAMESPACE_BEGIN
class VertexArray;
class Shader;
class Texture;
class Render2D
{
public:
	struct Render2DStroge
	{
		Ref<VertexArray> VertexArray;
		Ref<Shader> Shader;
		Ref<Texture> WhiteTexture;
	};

	static void Init();
	static void ShutDown();
	static void BeginScene(const Ref<OrthoGraphicCamera>& spOrthoGraphicCamera);
	static void EndScene();

	static void DrawQuad(const glm::vec2& vec2Position, float fRotation, const glm::vec2& vec2Size, const glm::vec4& vec4Color);
	static void DrawQuad(const glm::vec3& vec3Position, float fRotation, const glm::vec2& vec2Size, const glm::vec4& vec4Color);
	static void DrawQuad(const glm::vec2& vec2Position, float fRotation, const glm::vec2& vec2Size, const Ref<Texture>& spTexture, 
		float fFactor = 1.0f, const glm::vec4& vec4Color = glm::vec4(1.0f));
	static void DrawQuad(const glm::vec3& vec3Position, float fRotation, const glm::vec2& vec2Size, const Ref<Texture>& spTexture, 
		float fFactor = 1.0f, const glm::vec4& vec4Color = glm::vec4(1.0f));
private:																																			   
	static Ref<Render2DStroge> m_spRender2DStroge;
};


SAND_TABLE_NAMESPACE_END

