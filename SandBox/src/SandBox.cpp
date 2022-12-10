#include "..\..\SandTable\src\SandTable\Core\Application.h"
#include <SandTable.h>

class SandBoxLayer :public SandTable::Layer
{
public:
	SandBoxLayer() :Layer("SandBox") {}
	void OnUpdate()override
	{
		LOG_DEV_INFO("SandBoxLayer::OnUpdate");
	}
	void OnEvent(SandTable::Event& event) override
	{
		LOG_DEV_INFO(event);
	}
};

class SandBox :public SandTable::Application
{
public:
	SandBox() {}
	~SandBox() {}
};

std::shared_ptr<SandTable::Application> SandTable::Application ::CreateApplication()
{
	static std::shared_ptr<Application> spSandBox(new Application());
	auto spSandBoxLayer = std::make_shared<SandBoxLayer>();
	spSandBox->PushLayer(spSandBoxLayer);
	return spSandBox;
}
