#include "SandBoxExampleLayer.h"
#include "SandBox2DLayer.h"
#include "SandTable/Core/EntryPoint.h"

class SandBox :public SandTable::Application
{
public:
	~SandBox() {}
};

void SandTable::Application::CreateApplication()
{
	auto spSandBox = Application::GetApplication();
	auto spSandBoxLayer = std::make_shared<SandBoxExampleLayer>();
	spSandBox->PushLayer(spSandBoxLayer);
}
