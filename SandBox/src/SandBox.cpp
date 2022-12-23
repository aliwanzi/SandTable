#include "SandBoxExampleLayer.h"
#include "SandBox2DLayer.h"
#include "SandTable/Core/EntryPoint.h"

void SandTable::Application::CreateApplication()
{
	auto spSandBox = Application::GetApplication();
	auto spSandBoxLayer = std::make_shared<SandBox2DLayer>();
	spSandBox->PushLayer(spSandBoxLayer);
}
