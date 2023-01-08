#include "pch.h"
#include "Render2D.h"
#include "SandTable/Render/VertexArray.h"
#include "SandTable/Render/Shader.h"
#include "SandTable/Render/RenderCommand.h"
#include "SandTable/Debug/Instrumentor.h"

SAND_TABLE_NAMESPACE_BEGIN

Ref<Render2D::Render2DStroge> Render2D::m_spRender2DStroge = CreateRef<Render2D::Render2DStroge>();

void Render2D::Init()
{
	SAND_TABLE_PROFILE_FUNCTION();
	//VBO
	m_spRender2DStroge->VertexArray = VertexArray::Create();

	std::vector<VertexBufferElement> vecBufferElement
	{
		{ VertexDataType::Float3 },
		{ VertexDataType::Float4 },
		{ VertexDataType::Float2 },
		{ VertexDataType::Float },
		{ VertexDataType::Float }
	};
	auto spVertexBufferLayout = CreateRef<VertexBufferLayout>(vecBufferElement);
	m_spRender2DStroge->VertexBuffer = VertexBuffer::Create(m_spRender2DStroge->MaxVertices * sizeof(Vertex));
	std::dynamic_pointer_cast<VertexBuffer>(m_spRender2DStroge->VertexBuffer)->SetVertexBufferLayout(spVertexBufferLayout);
	m_spRender2DStroge->VertexArray->AddVertexBuffer(m_spRender2DStroge->VertexBuffer);

	m_spRender2DStroge->VertexPosition[0] = glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
	m_spRender2DStroge->VertexPosition[1] = glm::vec4( 0.5f, -0.5f, 0.0f, 1.0f);
	m_spRender2DStroge->VertexPosition[2] = glm::vec4( 0.5f,  0.5f, 0.0f, 1.0f);
	m_spRender2DStroge->VertexPosition[3] = glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f);

	//EBO
	std::vector<unsigned int> vecIndices(m_spRender2DStroge->MaxIndices);

	int uiOffset(0);
	for (unsigned int i = 0; i < m_spRender2DStroge->MaxIndices; i += 6)
	{
		vecIndices[i + 0] = uiOffset + 0;
		vecIndices[i + 1] = uiOffset + 1;
		vecIndices[i + 2] = uiOffset + 2;

		vecIndices[i + 3] = uiOffset + 2;
		vecIndices[i + 4] = uiOffset + 3;
		vecIndices[i + 5] = uiOffset + 0;

		uiOffset += 4;
	}

	auto spIndexBuffer = IndexBuffer::Create(vecIndices);
	m_spRender2DStroge->VertexArray->SetIndexBuffer(spIndexBuffer);

	//Shader
	std::vector<ShaderInfo> vecColorShaderInfo
	{
		{ShaderType::VETEX_SHADER,"assets/shaders/texture_color.vs"},
		{ShaderType::FRAGMENT_SHADER,"assets/shaders/texture_color.fs"}
	};
	m_spRender2DStroge->Shader = Shader::Create(vecColorShaderInfo);

	std::vector<int> vecTextures(32);
	for (int i = 0; i < 32; i++)
	{
		vecTextures[i] = i;
	}
	m_spRender2DStroge->Shader->Bind();
	m_spRender2DStroge->Shader->SetIntArray("u_Textures", &vecTextures[0], vecTextures.size());
	m_spRender2DStroge->TextureCoord[0] = glm::vec2(0.f, 0.f);
	m_spRender2DStroge->TextureCoord[1] = glm::vec2(1.f, 0.f);
	m_spRender2DStroge->TextureCoord[2] = glm::vec2(1.f, 1.f);
	m_spRender2DStroge->TextureCoord[3] = glm::vec2(0.f, 1.f);

	int uiWiteTextureData = 0xffffffff;
	m_spRender2DStroge->WhiteTexture = Texture2D::Create(1, 1);
	m_spRender2DStroge->WhiteTexture->SetData(&uiWiteTextureData, sizeof(uiWiteTextureData));
}

void Render2D::ShutDown()
{
}

void Render2D::BeginScene(const Ref<Camera>& spCamera)
{
	m_spRender2DStroge->Shader->Bind();
	m_spRender2DStroge->Shader->SetMat4("ViewProjection", spCamera->GetViewProjectionMatrix());
	StartBatch();
}

void Render2D::BeginScene(const Ref<Camera>& spCamera, const glm::mat4& mat4Transform)
{
	m_spRender2DStroge->Shader->Bind();
	m_spRender2DStroge->Shader->SetMat4("ViewProjection", spCamera->GetViewProjectionMatrix() * mat4Transform);
	StartBatch();
}

void Render2D::EndScene()
{
	Flush();
}

void Render2D::Flush()
{
	for (auto iter : m_spRender2DStroge->TextureSlots)
	{
		iter.second->Bind(iter.first);
	}
	std::dynamic_pointer_cast<VertexBuffer>(m_spRender2DStroge->VertexBuffer)->SetData
	(&m_spRender2DStroge->Vertex[0], m_spRender2DStroge->Vertex.size() * sizeof(Vertex));
	RenderCommand::DrawVertex(m_spRender2DStroge->VertexArray, m_spRender2DStroge->QuadIndexCount);

	m_spRender2DStroge->Stats.DrawCalls++;
}

void Render2D::DrawQuad(const glm::vec2& vec2Position, float fRotation, const glm::vec2& vec2Size, const glm::vec4& vec4Color)
{
	DrawQuad(glm::vec3(vec2Position, 0.f), fRotation, vec2Size, vec4Color);
}

void Render2D::DrawQuad(const glm::vec3& vec3Position, float fRotation, const glm::vec2& vec2Size, const glm::vec4& vec4Color)
{
	glm::mat4 matTransform = glm::translate(glm::mat4(1.f), vec3Position)
		* glm::rotate(glm::mat4(1.f), fRotation, glm::vec3(0.f, 0.f, 1.f))
		* glm::scale(glm::mat4(1.f), glm::vec3(vec2Size, 1.f));

	DrawQuad(matTransform, vec4Color);
}

void Render2D::DrawQuad(const glm::vec2& vec2Position, float fRotation, const glm::vec2& vec2Size, const Ref<Texture>& spTexture, 
	float fFactor, const glm::vec4& vec4Color)
{
	DrawQuad(glm::vec3(vec2Position, 0.f), fRotation, vec2Size, spTexture, fFactor, vec4Color);
}

void Render2D::DrawQuad(const glm::vec3& vec3Position, float fRotation, const glm::vec2& vec2Size, const Ref<Texture>& spTexture, 
	float fFactor, const glm::vec4& vec4Color)
{
	glm::mat4 matTransform = glm::translate(glm::mat4(1.f), vec3Position)
		* glm::rotate(glm::mat4(1.f), glm::radians(fRotation), glm::vec3(0.f, 0.f, 1.f))
		* glm::scale(glm::mat4(1.f), glm::vec3(vec2Size, 1.f));

	DrawQuad(matTransform, spTexture, fFactor, vec4Color);
}

void Render2D::DrawQuad(const glm::vec2& vec2Position, float fRotation, const glm::vec2& vec2Size, const Ref<SubTexture2D>& spSubTexture2D,
	float fFactor, const glm::vec4& vec4Color)
{
	DrawQuad(glm::vec3(vec2Position, 0.f), fRotation, vec2Size, spSubTexture2D, fFactor, vec4Color);
}

void Render2D::DrawQuad(const glm::vec3& vec3Position, float fRotation, const glm::vec2& vec2Size, const Ref<SubTexture2D>& spSubTexture2D,
	float fFactor, const glm::vec4& vec4Color)
{
	auto spRefTexture2D = spSubTexture2D->GetTexture();
	glm::mat4 matTransform = glm::translate(glm::mat4(1.f), vec3Position)
		* glm::rotate(glm::mat4(1.f), glm::radians(fRotation), glm::vec3(0.f, 0.f, 1.f))
		* glm::scale(glm::mat4(1.f), glm::vec3(vec2Size, 1.f));

	DrawQuad(matTransform, spRefTexture2D, fFactor, vec4Color);
}

void Render2D::DrawQuad(const glm::mat4& mat4Transform, const glm::vec4& vec4Color)
{
	if (m_spRender2DStroge->QuadIndexCount + 6 > m_spRender2DStroge->MaxIndices)
	{
		NextBatch();
	}
	for (int i = 0; i < 4; i++)
	{
		Vertex vertex;
		vertex.Position = mat4Transform * m_spRender2DStroge->VertexPosition[i];
		vertex.Color = vec4Color;
		vertex.TexCoord = m_spRender2DStroge->TextureCoord[i];
		vertex.TexIndex = m_spRender2DStroge->WhiteTexture->GetRenderID();
		vertex.TilingFactor = 1.0f;
		m_spRender2DStroge->Vertex.emplace_back(vertex);
	}

	m_spRender2DStroge->QuadIndexCount += 6;

	m_spRender2DStroge->Stats.QuadCount++;
}

void Render2D::DrawQuad(const glm::mat4& mat4Transform, const Ref<Texture>& spTexture, float fFactor, const glm::vec4& vec4Color)
{
	if (m_spRender2DStroge->TextureSlots.find(spTexture->GetRenderID())
		== m_spRender2DStroge->TextureSlots.end())
	{
		m_spRender2DStroge->TextureSlots[spTexture->GetRenderID()] = spTexture;
	}
	if (m_spRender2DStroge->TextureSlots.size() == 31  //Texture0Î´Ê¹ÓÃ
		|| m_spRender2DStroge->QuadIndexCount + 6 > m_spRender2DStroge->MaxIndices)
	{
		NextBatch();
	}

	for (int i = 0; i < 4; i++)
	{
		Vertex vertex;
		vertex.Position = mat4Transform * m_spRender2DStroge->VertexPosition[i];
		vertex.Color = vec4Color;
		vertex.TexCoord = m_spRender2DStroge->TextureCoord[i];
		vertex.TexIndex = spTexture->GetRenderID();
		vertex.TilingFactor = fFactor;
		m_spRender2DStroge->Vertex.emplace_back(vertex);
	}

	m_spRender2DStroge->QuadIndexCount += 6;
	m_spRender2DStroge->Stats.QuadCount++;
}


void Render2D::ResetStats()
{
	m_spRender2DStroge->Stats.DrawCalls = m_spRender2DStroge->Stats.QuadCount = 0;
}

Render2D::Statistics Render2D::GetStats()
{
	return m_spRender2DStroge->Stats;
}

void Render2D::StartBatch()
{
	m_spRender2DStroge->QuadIndexCount = 0;
	m_spRender2DStroge->Vertex.clear();
	m_spRender2DStroge->Vertex.reserve(m_spRender2DStroge->MaxVertices);
	m_spRender2DStroge->TextureSlots.clear();
	m_spRender2DStroge->TextureSlots[m_spRender2DStroge->WhiteTexture->GetRenderID()] = m_spRender2DStroge->WhiteTexture;
}

void Render2D::NextBatch()
{
	Flush();
	StartBatch();
}

SAND_TABLE_NAMESPACE_END
