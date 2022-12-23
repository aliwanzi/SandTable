#include "pch.h"
#include "Render2D.h"
#include "SandTable/Render/VertexArray.h"
#include "SandTable/Render/Shader.h"
#include "SandTable/Render/RenderCommand.h"
#include "SandTable/Render/Texture/Texture2D.h"

SAND_TABLE_NAMESPACE_BEGIN

Ref<Render2D::Render2DStroge> Render2D::m_spRender2DStroge = CreateRef<Render2D::Render2DStroge>();

void Render2D::Init()
{
	//VBO
	m_spRender2DStroge->VertexArray = VertexArray::Create();
	std::vector<float> vecVertex{
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};
	std::vector<VertexBufferElement> vecBufferElement
	{
		{ VertexDataType::Float3 },
		{ VertexDataType::Float2 }
	};
	auto spVertexBufferLayout = CreateRef<VertexBufferLayout>(vecBufferElement);
	auto spVertexBuffer = VertexBuffer::Create(vecVertex, spVertexBufferLayout);
	m_spRender2DStroge->VertexArray->AddVertexBuffer(spVertexBuffer);

	//EBO
	std::vector<int> vecIndex{
		0, 1, 2, 2, 3, 0
	};
	auto spIndexBuffer = IndexBuffer::Create(vecIndex);
	m_spRender2DStroge->VertexArray->SetIndexBuffer(spIndexBuffer);

	//Shader
	std::vector<ShaderInfo> vecColorShaderInfo
	{
		{ShaderType::VETEX_SHADER,"assets/shaders/texture_color.vs"},
		{ShaderType::FRAGMENT_SHADER,"assets/shaders/texture_color.fs"}
	};
	m_spRender2DStroge->Shader = Shader::Create(vecColorShaderInfo);

	int uiWiteTextureData = 0xffffffff;
	m_spRender2DStroge->WhiteTexture = Texture2D::Create(1, 1);
	m_spRender2DStroge->WhiteTexture->SetData(&uiWiteTextureData, sizeof(uiWiteTextureData));
}

void Render2D::ShutDown()
{
}

void Render2D::BeginScene(const Ref<OrthoGraphicCamera>& spOrthoGraphicCamera)
{
	m_spRender2DStroge->Shader->Bind();
	m_spRender2DStroge->Shader->SetMat4("ViewProjection", spOrthoGraphicCamera->GetViewProjectionMatrix());
}

void Render2D::EndScene()
{
}

void Render2D::DrawQuad(const glm::vec2& vec2Position, float fRotation, const glm::vec2& vec2Size, const glm::vec4& vec4Color)
{
	DrawQuad(glm::vec3(vec2Position, 1.f), fRotation, vec2Size, vec4Color);
}

void Render2D::DrawQuad(const glm::vec3& vec3Position, float fRotation, const glm::vec2& vec2Size, const glm::vec4& vec4Color)
{
	m_spRender2DStroge->Shader->Bind();
	m_spRender2DStroge->Shader->SetFloat4("Color", vec4Color);
	m_spRender2DStroge->WhiteTexture->Bind();

	glm::mat4 matTransform = glm::translate(glm::mat4(1.f), vec3Position)
		* glm::rotate(glm::mat4(1.f), glm::radians(fRotation), glm::vec3(0.f, 0.f, 1.f))
		* glm::scale(glm::mat4(1.f), glm::vec3(vec2Size, 1.f));
	m_spRender2DStroge->Shader->SetMat4("Transform", matTransform);

	m_spRender2DStroge->VertexArray->Bind();
	RenderCommand::DrawVertex(m_spRender2DStroge->VertexArray);
}

void Render2D::DrawQuad(const glm::vec2& vec2Position, float fRotation, const glm::vec2& vec2Size, const Ref<Texture>& spTexture)
{
	DrawQuad(glm::vec3(vec2Position, 1.f), fRotation, vec2Size, spTexture);
}

void Render2D::DrawQuad(const glm::vec3& vec3Position, float fRotation, const glm::vec2& vec2Size, const Ref<Texture>& spTexture)
{
	m_spRender2DStroge->Shader->Bind();
	m_spRender2DStroge->Shader->SetFloat4("Color", glm::vec4(1.0f));
	spTexture->Bind();

	glm::mat4 matTransform = glm::translate(glm::mat4(1.f), vec3Position)
		* glm::rotate(glm::mat4(1.f), glm::radians(fRotation), glm::vec3(0.f, 0.f, 1.f))
		* glm::scale(glm::mat4(1.f), glm::vec3(vec2Size, 1.f));
	m_spRender2DStroge->Shader->SetMat4("Transform", matTransform);
	m_spRender2DStroge->VertexArray->Bind();
	RenderCommand::DrawVertex(m_spRender2DStroge->VertexArray);
	spTexture->UnBind();
}

SAND_TABLE_NAMESPACE_END
