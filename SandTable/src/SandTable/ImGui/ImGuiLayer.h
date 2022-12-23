#pragma once
#include "SandTable/Core/Core.h"
#include "SandTable/Core/Layer.h"

SAND_TABLE_NAMESPACE_BEGIN

class ImGuiLayer :public Layer
{
public:
	ImGuiLayer();
	~ImGuiLayer();
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void BeginNewFrame();
	void EndNewFrame();
private:
	bool m_bShowDemoWindow;
	float m_fTime;
};

SAND_TABLE_NAMESPACE_END