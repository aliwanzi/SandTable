#pragma once

#include "SandTable/Core/Core.h"
#include "SandTable/Core/KeyCode.h"
#include "SandTable/Core/MouseCode.h"
#include <glm/glm.hpp>

SAND_TABLE_NAMESPACE_BEGIN

class Input
{
public:
	//Key
	static bool IsKeyPressed(KeyCode keyCode);

	//MouseButton
	static bool IsMouseButtonPressed(KeyCode keyCode);
	static glm::vec2 GetMousePos();
	static float GetMouseXPos();
	static float GetMouseYPos();

protected:
	virtual bool IsKeyPressedImpl(KeyCode keyCode) = 0;
	virtual bool IsMouseButtonPressedImpl(MouseCode mouseCode) = 0;
	virtual glm::vec2 GetMousePosImpl() = 0;
	virtual float GetMouseXPosImpl() = 0;
	virtual float GetMouseYPosImpl() = 0;

private:
	static Ref<Input> m_spInput;
};

SAND_TABLE_NAMESPACE_END