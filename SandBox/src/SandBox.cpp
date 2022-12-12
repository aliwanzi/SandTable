#include <SandTable.h>
#include <imgui/imgui.h>

class SandBoxLayer :public SandTable::Layer
{
public:
	SandBoxLayer() :Layer("SandBox") {}
	~SandBoxLayer() {}
	void OnImGuiRender()
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
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
	spSandBox->PushLayer(spSandBoxLayer);
}
