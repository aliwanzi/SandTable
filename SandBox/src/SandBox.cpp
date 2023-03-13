#include "EditorLayer/RayTracingEditorLayer.h"
#include "EditorLayer/SandBoxEditorLayer.h"
#include "SandTable/Core/EntryPoint.h"

void Application::CreateApplication(int argc, char** argv)
{
	auto spApplicationSpecification = CreateRef<ApplicationSpecification>();
	spApplicationSpecification->Name = "SandBox";
	spApplicationSpecification->CommandLineArgs = { argc, argv };
	auto spApplication = Application::GetApplication(spApplicationSpecification);

	auto spSandBoxLayer = std::make_shared<RayTracingEditorLayer>();
	spApplication->PushLayer(spSandBoxLayer);
}
