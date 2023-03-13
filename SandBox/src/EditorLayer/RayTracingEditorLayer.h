#pragma once
#include "SandTable.h"
#include <imgui/imgui.h>


class RayTracingEditorLayer :public Layer
{
public:
	RayTracingEditorLayer();
	virtual void OnAttach() override;
	virtual void OnImGuiRender() override;
private:
	void Render();
private:
	ImVec2 m_vec2RenderViewPortSize;
	Ref<Image> m_spImage;

	Ref<Timer> m_spTimer;
	float m_fLastRenderTime;
};

