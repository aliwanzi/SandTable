
#include "SandBoxEditorLayer.h"
#include "SandTable/Core/EntryPoint.h"

void SandTable::Application::CreateApplication()
{
	auto spSandBox = Application::GetApplication();
	auto spSandBoxLayer = std::make_shared<SandBoxEditorLayer>();
	spSandBox->PushLayer(spSandBoxLayer);
}
