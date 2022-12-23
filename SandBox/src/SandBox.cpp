#include "SandBox.h"
#include <glm/gtc/type_ptr.hpp>

SandBoxLayer::SandBoxLayer() :
	Layer("SandBox"),
	m_vec3Color(glm::vec3(0.2f,0.3f,0.8f))
{
	m_spOrthoGraphicCameraController = CreateRef<OrthoGraphicCameraController>
		(static_cast<float>(Application::GetApplication()->GetWindowWidth()) /
			static_cast<float>(Application::GetApplication()->GetWindowHeight()), true);
	//Texture
	//VAO
	m_spTextureArray = VertexArray::Create();

	//VBO
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
	m_spTextureArray->AddVertexBuffer(spVertexBuffer);

	//EBO
	std::vector<int> vecIndex{
		0, 1, 2, 2, 3, 0
	};
	auto spIndexBuffer = IndexBuffer::Create(vecIndex);
	m_spTextureArray->SetIndexBuffer(spIndexBuffer);

	//Shader
	std::vector<ShaderInfo> vecTriShaderInfo
	{ 
		{ShaderType::VETEX_SHADER,"assets/shaders/texture.vs"},
		{ShaderType::FRAGMENT_SHADER,"assets/shaders/texture.fs"}
	};
	m_spTextureShader = Shader::Create(vecTriShaderInfo);

	//Texture
	m_spTexture2DRGB = Texture2D::Create("assets/textures/Checkerboard.png");
	m_spTexture2DRGBA = Texture2D::Create("assets/textures/ChernoLogo.png");
	m_spTextureShader->Bind();
	m_spTextureShader->SetInt("tex", 0);

	//Color
	//VAO
	m_spColorArray = VertexArray::Create();

	//VBO
	std::vector<float> veColorVertex{
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};
	std::vector<VertexBufferElement> vecColorBufferElement
	{
		{ VertexDataType::Float3 },
		{ VertexDataType::Float2 }
	};
	auto spColorVertexBufferLayout = CreateRef<VertexBufferLayout>(vecColorBufferElement);
	auto spColorVertexBuffer = VertexBuffer::Create(veColorVertex, spColorVertexBufferLayout);
	m_spColorArray->AddVertexBuffer(spColorVertexBuffer);

	//EBO
	m_spColorArray->SetIndexBuffer(spIndexBuffer);

	//Shader
	std::vector<ShaderInfo> vecColorShaderInfo
	{
		{ShaderType::VETEX_SHADER,"assets/shaders/color.vs"},
		{ShaderType::FRAGMENT_SHADER,"assets/shaders/color.fs"}
	};
	m_spColorShader = Shader::Create(vecColorShaderInfo);
}

void SandBoxLayer::OnAttach()
{

}

void SandBoxLayer::OnDetach()
{

}

void SandBoxLayer::OnUpdate(const TimeStep& timeStep)
{
	m_spOrthoGraphicCameraController->OnUpdate(timeStep);

	RenderCommand::SetClearColor(glm::vec4(glm::vec3(0.1f), 1.0f));
	RenderCommand::Clear();


	Render::BeginScene(m_spOrthoGraphicCameraController->GetCamera());
	m_spColorShader->Bind();
	m_spColorShader->SetFloat3("Color", m_vec3Color);
	glm::mat4 mat4Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	for (int i = -10; i < 10; i++)
	{
		for (int j = -10; j < 10; j++)
		{
			glm::vec3 vec3Pos(i * 0.11f, j * 0.11f, 0.f);
			glm::mat4 mat4Transform = glm::translate(glm::mat4(1.0f), vec3Pos) * mat4Scale;

			Render::Submit(m_spColorShader, m_spColorArray, mat4Transform);
		}
	}
	m_spTexture2DRGB->Bind();
	Render::Submit(m_spTextureShader, m_spTextureArray);
	m_spTexture2DRGBA->Bind();
	Render::Submit(m_spTextureShader, m_spTextureArray);
	Render::EndScene();
}

void SandBoxLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_vec3Color));
	ImGui::End();
}

void SandBoxLayer::OnEvent(Event& event)
{
	m_spOrthoGraphicCameraController->OnEvent(event);
}


class SandBox :public SandTable::Application
{
public:
	~SandBox() {}
};

void SandTable::Application::CreateApplication()
{
	auto spSandBox = Application::GetApplication();
	auto spSandBoxLayer = std::make_shared<SandBoxLayer>();
	spSandBox->PushLayer(spSandBoxLayer);
}
