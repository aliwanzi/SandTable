#pragma once
#include "SandTable/Render/Camera/OrthoGraphicCameraController.h"

SAND_TABLE_NAMESPACE_BEGIN
class VertexArray;
class Buffer;
class Shader;
class Texture;

class Render2D
{
public:
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
	};

	struct Render2DStroge
	{
		unsigned int QuadIndexCount = 0;
		const unsigned int MaxQuads = 10000;
		const unsigned int MaxVertices = MaxQuads * 4;
		const unsigned int MaxIndices = MaxQuads * 6;

		Ref<VertexArray> VertexArray;
		std::vector<Vertex> Vertex;
		Ref<Buffer> VertexBuffer;
		Ref<Shader> Shader;
		Ref<Texture> WhiteTexture;
	};

	static void Init();
	static void ShutDown();
	static void BeginScene(const Ref<OrthoGraphicCamera>& spOrthoGraphicCamera);
	static void EndScene();
	static void Flush();

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

