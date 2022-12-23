#pragma once
#include "SandTable.h"
#include <imgui/imgui.h>
using namespace SandTable;

class SandBox2DLayer :public Layer
{
public:
	SandBox2DLayer();
	~SandBox2DLayer();
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(const TimeStep& timeStep) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Event& e) override;
private:
	Ref<OrthoGraphicCameraController> m_spOrthoGraphicCameraController;
	Ref<Texture> m_spTexture;

	glm::vec4 m_vec4Color;
};

