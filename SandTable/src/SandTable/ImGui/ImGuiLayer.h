#pragma once
#include "SandTable/Core/Layer.h"

SAND_TABLE_NAMESPACE_BEGIN

class ImGuiLayer :public Layer
{
public:
	ImGuiLayer();
	~ImGuiLayer();
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(Event&) override;
	void BeginNewFrame();
	void EndNewFrame();
	void BlockEvents(bool bBlock);
private:
	void SetDarkThemeColors();
private:
	bool m_bBlockEvents;
};

SAND_TABLE_NAMESPACE_END