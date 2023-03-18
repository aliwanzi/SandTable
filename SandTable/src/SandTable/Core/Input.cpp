#include "pch.h"
#include "Input.h"

SAND_TABLE_NAMESPACE_BEGIN

bool Input::IsKeyPressed(KeyCode keyCode)
{
	return m_spInput->IsKeyPressedImpl(keyCode);
}

bool Input::IsMouseButtonPressed(KeyCode keyCode)
{
	return m_spInput->IsMouseButtonPressedImpl(keyCode);
}

glm::vec2 Input::GetMousePos()
{
	return m_spInput->GetMousePosImpl();
}

float Input::GetMouseXPos()
{
	return m_spInput->GetMouseXPosImpl();
}

float Input::GetMouseYPos()
{
	return m_spInput->GetMouseYPosImpl();
}

void Input::SetCursorMode(CursorMode mode)
{
	m_spInput->SetCursorModeImpl(mode);
}

SAND_TABLE_NAMESPACE_END