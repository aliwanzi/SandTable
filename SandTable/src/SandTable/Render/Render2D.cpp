#include "pch.h"
#include "Render2D.h"
#include "SandTable/Render/VertexArray.h"
#include "SandTable/Render/Shader.h"
#include "SandTable/Render/RenderCommand.h"
#include "SandTable/Render/Buffer/UniformBuffer.h"
#include "SandTable/Render/Buffer/VertexBuffer.h"
#include "SandTable/Render/Buffer/IndexBuffer.h"
#include "SandTable/Render/Texture/Texture2D.h"

#include "SandTable/Debug/Instrumentor.h"
#include "SandTable/Debug/DrawStatistics/QuadDrawStatics.h"
#include "SandTable/Debug/DrawStatistics/CircleDrawStatics.h"

#include "SandTable/Scene/Components.h"

SAND_TABLE_NAMESPACE_BEGIN

Ref<Render2D::Render2DStroge> Render2D::m_spRender2DStroge = CreateRef<Render2D::Render2DStroge>();

void Render2D::Init()
{
	SAND_TABLE_PROFILE_FUNCTION();

	//QUAD
	m_spRender2DStroge->spQuadDrawStatics = CreateRef<QuadDrawStatics>(20000);
	m_spRender2DStroge->vecQuadVertexs.clear();

	auto spQuadVertexBufferLayout = CreateRef<VertexBufferLayout>(vecQuadBufferElement);
	auto iQuadVertexSize = spQuadVertexBufferLayout->GetStride();
	auto iQuadMaxVertices = m_spRender2DStroge->spQuadDrawStatics->GetMaxVertices();
	m_spRender2DStroge->spQuadVertexBuffer = VertexBuffer::Create(iQuadMaxVertices * iQuadVertexSize);
	auto spQuadVertexBuffer = std::dynamic_pointer_cast<VertexBuffer>(m_spRender2DStroge->spQuadVertexBuffer);
	spQuadVertexBuffer->SetVertexBufferLayout(spQuadVertexBufferLayout);

	m_spRender2DStroge->spQuadVertexArray = VertexArray::Create();
	m_spRender2DStroge->spQuadVertexArray->AddVertexBuffer(m_spRender2DStroge->spQuadVertexBuffer);

	std::vector<unsigned int> vecIndices(m_spRender2DStroge->spQuadDrawStatics->GetMaxIndices());

	int uiOffset(0);
	for (unsigned int i = 0; i < vecIndices.size(); i += 6)
	{
		vecIndices[i + 0] = uiOffset + 0;
		vecIndices[i + 1] = uiOffset + 1;
		vecIndices[i + 2] = uiOffset + 2;

		vecIndices[i + 3] = uiOffset + 2;
		vecIndices[i + 4] = uiOffset + 3;
		vecIndices[i + 5] = uiOffset + 0;

		uiOffset += 4;
	}

	auto spQuadIndexBuffer = IndexBuffer::Create(vecIndices);
	m_spRender2DStroge->spQuadVertexArray->SetIndexBuffer(spQuadIndexBuffer);

	std::vector<int> vecTextures(m_spRender2DStroge->spQuadDrawStatics->GetMaxTextureSlots());
	for (int i = 0; i < vecTextures.size(); i++)
	{
		vecTextures[i] = i;
	}

	int uiWiteTextureData = 0xffffffff;
	m_spRender2DStroge->spWhiteTexture = Texture2D::Create(1, 1);
	m_spRender2DStroge->spWhiteTexture->SetData(&uiWiteTextureData, sizeof(uiWiteTextureData));

	m_spRender2DStroge->CameraUniformBuffer = std::dynamic_pointer_cast<UniformBuffer>
		(UniformBuffer::Create(sizeof(CameraMatrix), 2));

	//Shader
	std::vector<ShaderInfo> vecQuadShaderInfo
	{
		{ShaderType::VETEX_SHADER,"assets/shaders/Render2D_Quad.vs"},
		{ShaderType::FRAGMENT_SHADER,"assets/shaders/Render2D_Quad.fs"}
	};
	m_spRender2DStroge->spQuadShader = Shader::Create(vecQuadShaderInfo);

	m_spRender2DStroge->spQuadShader->Bind();
	m_spRender2DStroge->spQuadShader->SetIntArray("u_Textures", vecTextures.data(), vecTextures.size());
	m_spRender2DStroge->spQuadShader->SetUniformBlock("Camera", 2);

	////Circle
	m_spRender2DStroge->spCircleDrawStatics = CreateRef<CircleDrawStatics>(20000);
	m_spRender2DStroge->vecCircleVertexs.clear();

	auto spCircleVertexBufferLayout = CreateRef<VertexBufferLayout>(vecCircleBufferElement);
	auto iCircleVertexSize = spCircleVertexBufferLayout->GetStride();
	auto iCircleMaxVertices = m_spRender2DStroge->spCircleDrawStatics->GetMaxVertices();
	m_spRender2DStroge->spCircleVertexBuffer = VertexBuffer::Create(iCircleMaxVertices * iCircleVertexSize);
	auto spCircleVertexBuffer = std::dynamic_pointer_cast<VertexBuffer>(m_spRender2DStroge->spCircleVertexBuffer);
	spCircleVertexBuffer->SetVertexBufferLayout(spCircleVertexBufferLayout);

	m_spRender2DStroge->spCircleVertexArray = VertexArray::Create();
	m_spRender2DStroge->spCircleVertexArray->AddVertexBuffer(m_spRender2DStroge->spCircleVertexBuffer);
	auto spCircleIndexBuffer = IndexBuffer::Create(vecIndices);
	m_spRender2DStroge->spCircleVertexArray->SetIndexBuffer(spCircleIndexBuffer);

	//Shader
	std::vector<ShaderInfo> vecCircleShaderInfo
	{
		{ShaderType::VETEX_SHADER,"assets/shaders/Render2D_Circle.vs"},
		{ShaderType::FRAGMENT_SHADER,"assets/shaders/Render2D_Circle.fs"}
	};
	m_spRender2DStroge->spCircleShader = Shader::Create(vecCircleShaderInfo);
	m_spRender2DStroge->spCircleShader->Bind();
	m_spRender2DStroge->spCircleShader->SetUniformBlock("Camera", 2);
}

void Render2D::ShutDown()
{
}

void Render2D::BeginScene(const Ref<Camera>& spCamera)
{
	m_spRender2DStroge->CameraBuffer.ViewProjection = spCamera->GetViewProjectionMatrix();
	auto spCameraUniformBuffer = std::dynamic_pointer_cast<UniformBuffer>(m_spRender2DStroge->CameraUniformBuffer);
	spCameraUniformBuffer->SetData(&m_spRender2DStroge->CameraBuffer, sizeof(CameraMatrix));
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

	if (m_spRender2DStroge->spQuadDrawStatics->GetDrawCount())
	{
		m_spRender2DStroge->spQuadShader->Bind();
		auto spQuadVertexBuffer = std::dynamic_pointer_cast<VertexBuffer>(m_spRender2DStroge->spQuadVertexBuffer);
		auto vertexSize = spQuadVertexBuffer->GetVertexBufferLayout()->GetStride();
		spQuadVertexBuffer->SetData(m_spRender2DStroge->vecQuadVertexs.data(), m_spRender2DStroge->vecQuadVertexs.size() * vertexSize);
		RenderCommand::DrawVertex(m_spRender2DStroge->spQuadVertexArray,m_spRender2DStroge->spQuadDrawStatics->GetTotalIndexCount());
		m_spRender2DStroge->spQuadDrawStatics->AddDrawCalls();
	}

	if (m_spRender2DStroge->spCircleDrawStatics->GetDrawCount())
	{
		m_spRender2DStroge->spCircleShader->Bind();
		auto spCircleVertexBuffer = std::dynamic_pointer_cast<VertexBuffer>(m_spRender2DStroge->spCircleVertexBuffer);
		auto vertexSize = spCircleVertexBuffer->GetVertexBufferLayout()->GetStride();
		spCircleVertexBuffer->SetData(m_spRender2DStroge->vecCircleVertexs.data(), m_spRender2DStroge->vecCircleVertexs.size() * vertexSize);
		RenderCommand::DrawVertex(m_spRender2DStroge->spCircleVertexArray, m_spRender2DStroge->spCircleDrawStatics->GetTotalIndexCount());
		m_spRender2DStroge->spCircleDrawStatics->AddDrawCalls();
	}
}

void Render2D::DrawSprite(const glm::mat4& mat4Transform, const SpriteRenderComponent& spriteRenderComponent, int iEntityID)
{
	DrawPrimitive(mat4Transform, spriteRenderComponent.spQuadPrimitive, spriteRenderComponent.spTexture, iEntityID);
}

void Render2D::DrawCircle(const glm::mat4& mat4Transform, const CircleRenderComponent& circleRenderComponent, int iEntityID)
{
	DrawPrimitive(mat4Transform, circleRenderComponent.spCirclePrimitive, nullptr, iEntityID);
}

void Render2D::DrawPrimitive(const glm::vec3& vec3Pan, float fRotation, const glm::vec3& vec3Scale, Ref<Primitive> spPrimitive, Ref<Texture> spTexture, int iEntityID)
{
	glm::mat4 matTransform = glm::translate(glm::mat4(1.f), vec3Pan)
		* glm::rotate(glm::mat4(1.f), fRotation, glm::vec3(0.f, 0.f, 1.f))
		* glm::scale(glm::mat4(1.f), vec3Scale);

	auto spQuadPrimitve = std::dynamic_pointer_cast<QuadPrimitive>(spPrimitive);
	if (spQuadPrimitve != nullptr)
	{
		DrawPrimitive(matTransform, spQuadPrimitve, spTexture, iEntityID);
	}

	auto spCirclePrimitve = std::dynamic_pointer_cast<CirclePrimitive>(spPrimitive);
	if (spCirclePrimitve != nullptr)
	{
		DrawPrimitive(matTransform, spCirclePrimitve, spTexture, iEntityID);
	}
}

void Render2D::ResetStats()
{
	m_spRender2DStroge->spQuadDrawStatics->ResetDrawCalls();
	m_spRender2DStroge->spQuadDrawStatics->ResetDrawCount();
	m_spRender2DStroge->spCircleDrawStatics->ResetDrawCalls();
	m_spRender2DStroge->spCircleDrawStatics->ResetDrawCount();
}

const Ref<QuadDrawStatics>& Render2D::GetQuadStatic()
{
	return m_spRender2DStroge->spQuadDrawStatics;
}

void Render2D::DrawPrimitive(const glm::mat4& mat4Transform, Ref<QuadPrimitive> spPrimitive, Ref<Texture> spTexture, int iEntityID)
{
	if (spTexture != nullptr)
	{
		if (m_spRender2DStroge->TextureSlots.find(spTexture->GetRenderID())
			== m_spRender2DStroge->TextureSlots.end())
		{
			m_spRender2DStroge->TextureSlots[spTexture->GetRenderID()] = spTexture;
		}
	}

	if (m_spRender2DStroge->TextureSlots.size() + 1 == m_spRender2DStroge->spQuadDrawStatics->GetMaxTextureSlots()  //Texture0未使用
		|| m_spRender2DStroge->spQuadDrawStatics->GetDrawCount() + 1 == m_spRender2DStroge->spQuadDrawStatics->GetMaxDrawCount())
	{
		NextBatch();
	}

	for (int i = 0; i < 4; i++)
	{
		QuadPrimitive::QuadVertex quadVertex;
		quadVertex.Position = mat4Transform * spPrimitive->GetPosition()[i];
		quadVertex.Color = spPrimitive->GetColor();
		quadVertex.TexCoord = spPrimitive->GetTexCoord()[i];
		quadVertex.TexIndex = spTexture != nullptr ? spTexture->GetRenderID() : m_spRender2DStroge->spWhiteTexture->GetRenderID();
		quadVertex.TilingFactor = spPrimitive->GetTilingFactor();
		quadVertex.EntityID = iEntityID;
		m_spRender2DStroge->vecQuadVertexs.emplace_back(quadVertex);
	}

	m_spRender2DStroge->spQuadDrawStatics->AddDrawCount();
}

void Render2D::DrawPrimitive(const glm::mat4& mat4Transform, Ref<CirclePrimitive> spPrimitive, Ref<Texture> spTexture, int iEntityID)
{
	if (spTexture != nullptr)
	{
		if (m_spRender2DStroge->TextureSlots.find(spTexture->GetRenderID())
			== m_spRender2DStroge->TextureSlots.end())
		{
			m_spRender2DStroge->TextureSlots[spTexture->GetRenderID()] = spTexture;
		}
	}

	if (m_spRender2DStroge->TextureSlots.size() + 1 == m_spRender2DStroge->spQuadDrawStatics->GetMaxTextureSlots()  //Texture0未使用
		|| m_spRender2DStroge->spCircleDrawStatics->GetDrawCount() + 1 == m_spRender2DStroge->spCircleDrawStatics->GetMaxDrawCount())
	{
		NextBatch();
	}

	for (int i = 0; i < 4; i++)
	{
		CirclePrimitive::CircleVertex circleVertex;
		circleVertex.WorldPosition = mat4Transform * spPrimitive->GetPosition()[i];
		circleVertex.LocalPosition = spPrimitive->GetPosition()[i] * 2.f;
		circleVertex.Color = spPrimitive->GetColor();
		circleVertex.Thickness = spPrimitive->GetThickness();
		circleVertex.Fade = spPrimitive->GetFade();
		circleVertex.EntityID = iEntityID;
		m_spRender2DStroge->vecCircleVertexs.emplace_back(circleVertex);
	}

	m_spRender2DStroge->spCircleDrawStatics->AddDrawCount();
}

void Render2D::StartBatch()
{
	m_spRender2DStroge->spQuadDrawStatics->ResetDrawCount();
	m_spRender2DStroge->vecQuadVertexs.clear();
	m_spRender2DStroge->vecQuadVertexs.reserve(m_spRender2DStroge->spQuadDrawStatics->GetMaxVertices());
	m_spRender2DStroge->TextureSlots.clear();
	m_spRender2DStroge->TextureSlots[m_spRender2DStroge->spWhiteTexture->GetRenderID()] = m_spRender2DStroge->spWhiteTexture;

	m_spRender2DStroge->spCircleDrawStatics->ResetDrawCount();
	m_spRender2DStroge->vecCircleVertexs.clear();
	m_spRender2DStroge->vecCircleVertexs.reserve(m_spRender2DStroge->spCircleDrawStatics->GetMaxVertices());
}

void Render2D::NextBatch()
{
	Flush();
	StartBatch();
}

SAND_TABLE_NAMESPACE_END
