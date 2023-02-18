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
#include "SandTable/Debug/DrawStatistics/DrawStatistics.h"
#include "SandTable/Debug/DrawStatistics/QuadDrawStatics.h"
#include "SandTable/Debug/DrawStatistics/CircleDrawStatics.h"
#include "SandTable/Debug/DrawStatistics/LineDrawStatics.h"

#include "SandTable/Scene/Components.h"

SAND_TABLE_NAMESPACE_BEGIN

Ref<Render2D::Render2DStroge> Render2D::m_spRender2DStroge = CreateRef<Render2D::Render2DStroge>();
const unsigned int MaxTextureSlots = 32;

void Render2D::CreateRenderPara(Ref<DrawStatistics> spDrawStatistics,
	Ref<VertexArray>& spVertexArray, Ref<Buffer>& spVertexBuffer, const std::vector<VertexBufferElement>& vertexBufferElement)
{
	unsigned int uiMaxVertices = spDrawStatistics->GetMaxVertices();
	unsigned int uiMaxIndices = spDrawStatistics->GetMaxIndices();

	auto spVertexBufferLayout = CreateRef<VertexBufferLayout>(vertexBufferElement);
	auto iVertexSize = spVertexBufferLayout->GetStride();
	spVertexBuffer = VertexBuffer::Create(uiMaxVertices * iVertexSize);
	auto& spBuffer = std::dynamic_pointer_cast<VertexBuffer>(spVertexBuffer);
	spBuffer->SetVertexBufferLayout(spVertexBufferLayout);

	spVertexArray = VertexArray::Create();
	spVertexArray->AddVertexBuffer(spVertexBuffer);

	auto spLineDrawStatistics = std::dynamic_pointer_cast<LineDrawStatics>(spDrawStatistics);
	if (spLineDrawStatistics == nullptr)
	{
		std::vector<unsigned int> vecIndices(uiMaxIndices);
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
		auto spIndexBuffer = IndexBuffer::Create(vecIndices);
		spVertexArray->SetIndexBuffer(spIndexBuffer);
	}
}

void Render2D::Init()
{
	SAND_TABLE_PROFILE_FUNCTION();

	std::vector<int> vecTextures(MaxTextureSlots);
	for (int i = 0; i < vecTextures.size(); i++)
	{
		vecTextures[i] = i;
	}

	int uiWiteTextureData = 0xffffffff;
	m_spRender2DStroge->spWhiteTexture = Texture2D::Create(1, 1);
	m_spRender2DStroge->spWhiteTexture->SetData(&uiWiteTextureData, sizeof(uiWiteTextureData));

	m_spRender2DStroge->CameraUniformBuffer = std::dynamic_pointer_cast<UniformBuffer>
		(UniformBuffer::Create(sizeof(CameraMatrix), 2));

	////Line
	m_spRender2DStroge->spLineDrawStatics = CreateRef<LineDrawStatics>(1000);
	m_spRender2DStroge->vecLineVertexs.reserve(m_spRender2DStroge->spLineDrawStatics->GetMaxVertices());
	CreateRenderPara(m_spRender2DStroge->spLineDrawStatics, m_spRender2DStroge->spLineVertexArray,
		m_spRender2DStroge->spLineVertexBuffer, vecLineBufferElement);

	std::vector<ShaderInfo> vecLineShaderInfo
	{
		{ShaderType::VETEX_SHADER,"assets/shaders/Render2D_Line.vs"},
		{ShaderType::FRAGMENT_SHADER,"assets/shaders/Render2D_Line.fs"}
	};
	m_spRender2DStroge->spLineShader = Shader::Create(vecLineShaderInfo);
	m_spRender2DStroge->spLineShader->Bind();
	m_spRender2DStroge->spLineShader->SetUniformBlock("Camera", 2);

	//QUAD
	m_spRender2DStroge->spQuadDrawStatics = CreateRef<QuadDrawStatics>(20000);
	m_spRender2DStroge->vecQuadVertexs.reserve(m_spRender2DStroge->spQuadDrawStatics->GetMaxVertices());
	CreateRenderPara(m_spRender2DStroge->spQuadDrawStatics, m_spRender2DStroge->spQuadVertexArray,
		m_spRender2DStroge->spQuadVertexBuffer, vecQuadBufferElement);

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
	m_spRender2DStroge->vecQuadVertexs.reserve(m_spRender2DStroge->spCircleDrawStatics->GetMaxVertices());
	CreateRenderPara(m_spRender2DStroge->spCircleDrawStatics, m_spRender2DStroge->spCircleVertexArray,
		m_spRender2DStroge->spCircleVertexBuffer, vecCircleBufferElement);

	std::vector<ShaderInfo> vecCircleShaderInfo
	{
		{ShaderType::VETEX_SHADER,"assets/shaders/Render2D_Circle.vs"},
		{ShaderType::FRAGMENT_SHADER,"assets/shaders/Render2D_Circle.fs"}
	};
	m_spRender2DStroge->spCircleShader = Shader::Create(vecCircleShaderInfo);
	m_spRender2DStroge->spCircleShader->Bind();
	m_spRender2DStroge->spCircleShader->SetUniformBlock("Camera", 2);


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
		SAND_TABLE_ASSERT(spQuadVertexBuffer, "QuadVertexBuffer is null in Render2D Draw line");
		auto vertexSize = spQuadVertexBuffer->GetVertexBufferLayout()->GetStride();
		spQuadVertexBuffer->SetData(m_spRender2DStroge->vecQuadVertexs.data(),
			m_spRender2DStroge->vecQuadVertexs.size() * vertexSize);
		RenderCommand::DrawTriangles(m_spRender2DStroge->spQuadVertexArray,
			m_spRender2DStroge->spQuadDrawStatics->GetTotalIndexCount());
		m_spRender2DStroge->spQuadDrawStatics->AddDrawCalls();
	}

	if (m_spRender2DStroge->spCircleDrawStatics->GetDrawCount())
	{
		m_spRender2DStroge->spCircleShader->Bind();
		auto spCircleVertexBuffer = std::dynamic_pointer_cast<VertexBuffer>(m_spRender2DStroge->spCircleVertexBuffer);
		SAND_TABLE_ASSERT(spCircleVertexBuffer, "CircleVertexBuffer is null in Render2D Draw line");
		auto vertexSize = spCircleVertexBuffer->GetVertexBufferLayout()->GetStride();
		spCircleVertexBuffer->SetData(m_spRender2DStroge->vecCircleVertexs.data(), 
			m_spRender2DStroge->vecCircleVertexs.size() * vertexSize);
		RenderCommand::DrawTriangles(m_spRender2DStroge->spCircleVertexArray, 
			m_spRender2DStroge->spCircleDrawStatics->GetTotalIndexCount());
		m_spRender2DStroge->spCircleDrawStatics->AddDrawCalls();
	}

	if (m_spRender2DStroge->spLineDrawStatics->GetDrawCount())
	{
		m_spRender2DStroge->spLineShader->Bind();
		auto spLineVertexBuffer = std::dynamic_pointer_cast<VertexBuffer>(m_spRender2DStroge->spLineVertexBuffer);
		SAND_TABLE_ASSERT(spLineVertexBuffer, "LineVertexBuffer is null in Render2D Draw line");
		auto vertexSize = spLineVertexBuffer->GetVertexBufferLayout()->GetStride();
		spLineVertexBuffer->SetData(m_spRender2DStroge->vecLineVertexs.data(),
			m_spRender2DStroge->vecLineVertexs.size() * vertexSize);
		RenderCommand::SetLineWidth(2.f);
		RenderCommand::DrawLines(m_spRender2DStroge->spLineVertexArray,
			m_spRender2DStroge->vecLineVertexs.size());
		m_spRender2DStroge->spLineDrawStatics->AddDrawCalls();
	}
}

void Render2D::DrawSprite(const glm::mat4& mat4Transform, const SpriteRenderComponent& spriteRenderComponent, int iEntityID)
{
	DrawPrimitive(mat4Transform, spriteRenderComponent.spQuadPrimitive, spriteRenderComponent.spTexture, iEntityID);
}

void Render2D::DrawCircle(const glm::mat4& mat4Transform, const CircleRenderComponent& circleRenderComponent, int iEntityID)
{
	DrawPrimitive(mat4Transform, circleRenderComponent.spCirclePrimitive, circleRenderComponent.spTexture, iEntityID);
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

	auto spLinePrimitive = std::dynamic_pointer_cast<LinePrimitive>(spPrimitive);
	if (spLinePrimitive != nullptr)
	{
		DrawPrimitive(matTransform, spLinePrimitive, spTexture, iEntityID);
	}
}

void Render2D::DrawPrimitive(const glm::mat4& mat4Transform, Ref<Primitive> spPrimitive, Ref<Texture> spTexture, int iEntityID)
{
	auto spQuadPrimitve = std::dynamic_pointer_cast<QuadPrimitive>(spPrimitive);
	if (spQuadPrimitve != nullptr)
	{
		DrawPrimitive(mat4Transform, spQuadPrimitve, spTexture, iEntityID);
	}

	auto spCirclePrimitve = std::dynamic_pointer_cast<CirclePrimitive>(spPrimitive);
	if (spCirclePrimitve != nullptr)
	{
		DrawPrimitive(mat4Transform, spCirclePrimitve, spTexture, iEntityID);
	}

	auto spLinePrimitive = std::dynamic_pointer_cast<LinePrimitive>(spPrimitive);
	if (spLinePrimitive != nullptr)
	{
		DrawPrimitive(mat4Transform, spLinePrimitive, spTexture, iEntityID);
	}
}

void Render2D::DrawBoundary(const glm::mat4& mat4Transform, Ref<Primitive> spPrimitive)
{
	auto spLinePrimitive = CreateRef<LinePrimitive>();
	const auto& vecPositions = spPrimitive->GetPositions();
	spLinePrimitive->SetColor(spPrimitive->GetColor());
	for (int i = 0; i < vecPositions.size(); i++)
	{
		spLinePrimitive->AddPosition(vecPositions[i]);
		if (i < vecPositions.size() - 1)
		{
			spLinePrimitive->AddPosition(vecPositions[i + 1]);
		}
		else
		{
			spLinePrimitive->AddPosition(vecPositions[0]);
		}
	}

	DrawPrimitive(mat4Transform, spLinePrimitive);
}

void Render2D::ResetStats()
{
	m_spRender2DStroge->spQuadDrawStatics->ResetDrawCalls();
	m_spRender2DStroge->spQuadDrawStatics->ResetDrawCount();
	m_spRender2DStroge->spCircleDrawStatics->ResetDrawCalls();
	m_spRender2DStroge->spCircleDrawStatics->ResetDrawCount();
	m_spRender2DStroge->spLineDrawStatics->ResetDrawCalls();
	m_spRender2DStroge->spLineDrawStatics->ResetDrawCount();
}

const Ref<DrawStatistics>& Render2D::GetQuadStatic()
{
	return m_spRender2DStroge->spQuadDrawStatics;
}

const Ref<DrawStatistics>& Render2D::GetCircleStatic()
{
	return m_spRender2DStroge->spCircleDrawStatics;
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

	if (m_spRender2DStroge->TextureSlots.size() + 1 == MaxTextureSlots  //Texture0未使用
		|| m_spRender2DStroge->spQuadDrawStatics->GetDrawCount() + 1 == m_spRender2DStroge->spQuadDrawStatics->GetMaxDrawCount())
	{
		NextBatch();
	}

	const auto& vecPositions = spPrimitive->GetPositions();
	for (int i = 0; i < vecPositions.size(); i++)
	{
		QuadPrimitive::QuadVertex quadVertex;
		quadVertex.Position = mat4Transform * vecPositions[i];
		quadVertex.Color = spPrimitive->GetColor();
		quadVertex.TexCoord = spPrimitive->GetTexCoord()[i];
		quadVertex.TexIndex = spTexture != nullptr ? spTexture->GetRenderID() : m_spRender2DStroge->spWhiteTexture->GetRenderID();
		quadVertex.TilingFactor = spPrimitive->GetTilingFactor();
		quadVertex.EntityID = iEntityID;
		m_spRender2DStroge->vecQuadVertexs.push_back(quadVertex);
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

	if (m_spRender2DStroge->TextureSlots.size() + 1 == MaxTextureSlots  //Texture0未使用
		|| m_spRender2DStroge->spCircleDrawStatics->GetDrawCount() + 1 == m_spRender2DStroge->spCircleDrawStatics->GetMaxDrawCount())
	{
		NextBatch();
	}

	const auto& vecPositions = spPrimitive->GetPositions();
	for (int i = 0; i < vecPositions.size(); i++)
	{
		CirclePrimitive::CircleVertex circleVertex;
		circleVertex.WorldPosition = mat4Transform * vecPositions[i];
		circleVertex.LocalPosition = vecPositions[i] * 2.f;
		circleVertex.Color = spPrimitive->GetColor();
		circleVertex.TexCoord = spPrimitive->GetTexCoord()[i];
		circleVertex.TexIndex = spTexture != nullptr ? spTexture->GetRenderID() : m_spRender2DStroge->spWhiteTexture->GetRenderID();
		circleVertex.Thickness = spPrimitive->GetThickness();
		circleVertex.Fade = spPrimitive->GetFade();
		circleVertex.EntityID = iEntityID;
		m_spRender2DStroge->vecCircleVertexs.push_back(circleVertex);
	}

	m_spRender2DStroge->spCircleDrawStatics->AddDrawCount();
}

void Render2D::DrawPrimitive(const glm::mat4& mat4Transform, Ref<LinePrimitive> spPrimitive, Ref<Texture> spTexture, int iEntityID)
{
	if (spTexture != nullptr)
	{
		if (m_spRender2DStroge->TextureSlots.find(spTexture->GetRenderID())
			== m_spRender2DStroge->TextureSlots.end())
		{
			m_spRender2DStroge->TextureSlots[spTexture->GetRenderID()] = spTexture;
		}
	}

	if (m_spRender2DStroge->TextureSlots.size() + 1 == MaxTextureSlots  //Texture0未使用
		|| m_spRender2DStroge->spLineDrawStatics->GetDrawCount() + 1 == m_spRender2DStroge->spLineDrawStatics->GetMaxDrawCount())
	{
		NextBatch();
	}

	auto vecPositions = spPrimitive->GetPositions();
	for (int i = 0; i < vecPositions.size(); i++)
	{
		LinePrimitive::LineVertex lineVertex;
		lineVertex.Position = mat4Transform * vecPositions[i];
		lineVertex.Color = spPrimitive->GetColor();
		lineVertex.EntityID = iEntityID;
		m_spRender2DStroge->vecLineVertexs.push_back(lineVertex);
		m_spRender2DStroge->spLineDrawStatics->AddDrawCount();
	}
}

void Render2D::StartBatch()
{
	m_spRender2DStroge->TextureSlots.clear();
	m_spRender2DStroge->TextureSlots[m_spRender2DStroge->spWhiteTexture->GetRenderID()] = m_spRender2DStroge->spWhiteTexture;

	m_spRender2DStroge->spQuadDrawStatics->ResetDrawCount();
	m_spRender2DStroge->vecQuadVertexs.clear();
	m_spRender2DStroge->vecQuadVertexs.reserve(m_spRender2DStroge->spQuadDrawStatics->GetMaxVertices());
	
	m_spRender2DStroge->spCircleDrawStatics->ResetDrawCount();
	m_spRender2DStroge->vecCircleVertexs.clear();
	m_spRender2DStroge->vecCircleVertexs.reserve(m_spRender2DStroge->spCircleDrawStatics->GetMaxVertices());

	m_spRender2DStroge->spLineDrawStatics->ResetDrawCount();
	m_spRender2DStroge->vecLineVertexs.clear();
	m_spRender2DStroge->vecLineVertexs.reserve(m_spRender2DStroge->spLineDrawStatics->GetMaxVertices());
}

void Render2D::NextBatch()
{
	Flush();
	StartBatch();
}

SAND_TABLE_NAMESPACE_END
