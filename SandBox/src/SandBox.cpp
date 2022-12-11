#include <SandTable.h>

class SandBoxLayer :public SandTable::Layer
{
public:
	SandBoxLayer() :Layer("SandBox") {}
	~SandBoxLayer() {}
	void OnUpdate()override
	{
		LOG_DEV_INFO("SandBoxLayer::OnUpdate");
	}
	void OnEvent(SandTable::Event& event) override
	{
		LOG_DEV_INFO(event);
	}
	void OnAttach()override
	{

	}
	void OnDetach() override
	{

	}

};

class SandBox :public SandTable::Application
{
public:
	~SandBox() {}
};

void SandTable::Application::CreateApplication()
{
	auto spSandBox = Application::GetApplication();
	auto spSandBoxLayer = std::make_shared<SandBoxLayer>();
	auto spImGuiLayer = std::make_shared<ImGuiLayer>();
	spSandBox->PushLayer(spSandBoxLayer);
	spSandBox->PushLayer(spImGuiLayer);
}
