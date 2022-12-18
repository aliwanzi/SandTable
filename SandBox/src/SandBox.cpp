#include "SandBox.h"
#include <glm/gtc/type_ptr.hpp>

SandBoxLayer::SandBoxLayer() :
	Layer("SandBox"),
	m_spOrthoGraphicCamera(CreateRef<OrthoGraphicCamera>(-1.6f, 1.6f, -0.9f, 0.9f)),
	m_vec3Color(glm::vec3(0.2f,0.3f,0.8f)),
	m_vec3CameraPosition(glm::vec3(0.f)),
	m_fCameraMoveSpeed(0.003),
	m_fCameraRotation(0.f),
	m_fCameraRotateSpeed(0.3f)
{
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
	m_spTexture2D = Texture2D::Create("assets/textures/Checkerboard.png");
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
	if (Input::IsKeyPressed(Key::W))
	{
		m_vec3CameraPosition.y += m_fCameraMoveSpeed * timeStep;
	}
	else if (Input::IsKeyPressed(Key::S))
	{
		m_vec3CameraPosition.y -= m_fCameraMoveSpeed * timeStep;
	}

	if (Input::IsKeyPressed(Key::A))
	{
		m_vec3CameraPosition.x -= m_fCameraMoveSpeed * timeStep;
	}
	else if (Input::IsKeyPressed(Key::D))
	{
		m_vec3CameraPosition.x += m_fCameraMoveSpeed * timeStep;
	}

	if (Input::IsKeyPressed(Key::C))
	{
		m_fCameraRotation -= m_fCameraRotateSpeed * timeStep;
	}
	else if(Input::IsKeyPressed(Key::Z))
	{
		m_fCameraRotation += m_fCameraRotateSpeed * timeStep;
	}

	m_spOrthoGraphicCamera->SetPosition(m_vec3CameraPosition);
	m_spOrthoGraphicCamera->SetRotation(m_fCameraRotation);

	RenderCommand::SetClearColor(glm::vec4(glm::vec3(0.1f), 1.0f));
	RenderCommand::Clear();

	Render::BeginScene(m_spOrthoGraphicCamera);
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
	m_spTexture2D->Bind();
	Render::Submit(m_spTextureShader, m_spTextureArray);
	Render::EndScene();
}

void SandBoxLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_vec3Color));
	ImGui::End();
}

void SandBoxLayer::OnEvent(Event& e)
{
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
