
#include "SandBoxEditorLayer.h"
#include "SandTable/Core/EntryPoint.h"

void SandTable::Application::CreateApplication()
{
	auto spApplicationSpecification = CreateRef<ApplicationSpecification>();
	auto spSandBox = Application::GetApplication(spApplicationSpecification);
	auto spSandBoxLayer = std::make_shared<SandBoxEditorLayer>();
	spSandBox->PushLayer(spSandBoxLayer);
}
