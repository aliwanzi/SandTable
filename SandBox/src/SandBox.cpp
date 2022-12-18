#include "SandBox.h"
#include <glm/gtc/type_ptr.hpp>

SandBoxLayer::SandBoxLayer() :
	Layer("SandBox"),
	m_spOrthoGraphicCamera(CreateRef<OrthoGraphicCamera>(-1.6f, 1.6f, -0.9f, 0.9f)),
	m_vec3Color(glm::vec3(0.2f,0.3f,0.8f)),
	m_vec3CameraPosition(glm::vec3(0.f)),
	m_fCameraMoveSpeed(0.01f),
	m_fCameraRotation(0.f),
	m_fCameraRotateSpeed(0.5f)
{
	//Triangle
	//VAO
	m_spTriangle = VertexArray::Create();

	//VBO
	std::vector<float> vecTriVertex{
		-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
	};
	std::vector<VertexBufferElement> vecTriBufferElement
	{
		{ VertexDataType::Float3 },
		{ VertexDataType::Float4 }
	};
	auto sTriVertexBufferLayout = CreateRef<VertexBufferLayout>(vecTriBufferElement);
	auto spTriVertexBuffer = VertexBuffer::Create(vecTriVertex, sTriVertexBufferLayout);
	m_spTriangle->AddVertexBuffer(spTriVertexBuffer);

	//EBO
	std::vector<int> vecTriIndex{
		0,1,2
	};
	auto spTriIndexBuffer = IndexBuffer::Create(vecTriIndex);
	m_spTriangle->SetIndexBuffer(spTriIndexBuffer);

	//Shader
	std::vector<ShaderInfo> vecTriShaderInfo
	{ 
		{ShaderType::VETEX_SHADER,"assets/shaders/Triangle.vs"},
		{ShaderType::FRAGMENT_SHADER,"assets/shaders/Triangle.fs"}
	};
	m_spTriangleShader = Shader::Create(vecTriShaderInfo);


	//Square
	//VAO
	m_spSquare = VertexArray::Create();

	//VBO
	std::vector<float> vecSquVertex{
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};
	std::vector<VertexBufferElement> vecSquBufferElement
	{
		{ VertexDataType::Float3 },
		{ VertexDataType::Float2 }
	};
	auto spSquVertexBufferLayout = CreateRef<VertexBufferLayout>(vecSquBufferElement);
	auto spSquVertexBuffer = VertexBuffer::Create(vecSquVertex, spSquVertexBufferLayout);
	m_spSquare->AddVertexBuffer(spSquVertexBuffer);

	//EBO
	std::vector<int> vecSquIndex{
		0, 1, 2, 2, 3, 0
	};
	auto spSquIndexBuffer = IndexBuffer::Create(vecSquIndex);
	m_spSquare->SetIndexBuffer(spSquIndexBuffer);

	//Shader
	std::vector<ShaderInfo> vecSquShaderInfo
	{
		{ShaderType::VETEX_SHADER,"assets/shaders/Square.vs"},
		{ShaderType::FRAGMENT_SHADER,"assets/shaders/Square.fs"}
	};
	m_spSquareShader = Shader::Create(vecSquShaderInfo);
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
	m_spSquareShader->Bind();
	m_spSquareShader->SetFloat3("Color", m_vec3Color);
	glm::mat4 mat4Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			glm::vec3 vec3Pos(i * 0.11f, j * 0.11f, 0.f);
			glm::mat4 mat4Transform = glm::translate(glm::mat4(1.0f), vec3Pos) * mat4Scale;

			Render::Submit(m_spSquareShader, m_spSquare, mat4Transform);
		}
	}

	Render::Submit(m_spTriangleShader, m_spTriangle);
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
