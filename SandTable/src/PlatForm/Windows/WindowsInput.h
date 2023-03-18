#pragma once
#include "SandTable/Core/Input.h"

SAND_TABLE_NAMESPACE_BEGIN

class WindowsInput :public Input
{
protected:
	virtual bool IsKeyPressedImpl(KeyCode keyCode) override;
	virtual bool IsMouseButtonPressedImpl(MouseCode mouseCode)override;
	virtual glm::vec2 GetMousePosImpl()override;
	virtual float GetMouseXPosImpl() override;
	virtual float GetMouseYPosImpl() override;

	virtual void SetCursorModeImpl(CursorMode mode)override;
};

SAND_TABLE_NAMESPACE_END