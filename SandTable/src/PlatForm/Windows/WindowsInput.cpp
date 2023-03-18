#include "pch.h"
#include "WindowsInput.h"
#include "SandTable/Core/Application.h"
#include "GLFW/glfw3.h"

SAND_TABLE_NAMESPACE_BEGIN

Ref<Input> Input::m_spInput = Ref<WindowsInput>(new WindowsInput());

bool WindowsInput::IsKeyPressedImpl(KeyCode keyCode)
{
	const auto& spWindow = Application::GetApplication()->GetWindow();
	SAND_TABLE_ASSERT(spWindow, "WindowsInput get window failed");
	auto glfwWindow = static_cast<GLFWwindow*>(spWindow->GetNativeWindow());
	SAND_TABLE_ASSERT(glfwWindow, "WindowsInput get glfwWindow failed");

	auto state = glfwGetKey(glfwWindow, keyCode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool WindowsInput::IsMouseButtonPressedImpl(MouseCode mouseCode)
{
	const auto& upWindow = Application::GetApplication()->GetWindow();
	SAND_TABLE_ASSERT(upWindow, "WindowsInput get window failed");
	auto glfwWindow = static_cast<GLFWwindow*>(upWindow->GetNativeWindow());
	SAND_TABLE_ASSERT(glfwWindow, "WindowsInput get glfwWindow failed");

	auto state = glfwGetMouseButton(glfwWindow, mouseCode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

glm::vec2 WindowsInput::GetMousePosImpl()
{
	const auto& upWindow = Application::GetApplication()->GetWindow();
	SAND_TABLE_ASSERT(upWindow, "WindowsInput get window failed");
	auto glfwWindow = static_cast<GLFWwindow*>(upWindow->GetNativeWindow());
	SAND_TABLE_ASSERT(glfwWindow, "WindowsInput get glfwWindow failed");

	double dXPos(0.0), dYPos(0.0);
	glfwGetCursorPos(glfwWindow, &dXPos, &dYPos);
	return glm::vec2(dXPos, dYPos);
}

float WindowsInput::GetMouseXPosImpl()
{
	auto vec2Pos = GetMousePosImpl();
	return vec2Pos.x;
}

float WindowsInput::GetMouseYPosImpl()
{
	auto vec2Pos = GetMousePosImpl();
	return vec2Pos.y;
}

void WindowsInput::SetCursorModeImpl(CursorMode mode)
{
	const auto& upWindow = Application::GetApplication()->GetWindow();
	SAND_TABLE_ASSERT(upWindow, "WindowsInput get window failed");
	auto glfwWindow = static_cast<GLFWwindow*>(upWindow->GetNativeWindow());
	SAND_TABLE_ASSERT(glfwWindow, "WindowsInput get glfwWindow failed");
	glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL + (int)mode);
}

SAND_TABLE_NAMESPACE_END