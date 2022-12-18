#pragma once
#include "SandTable/Render/Context.h"

struct GLFWwindow;
SAND_TABLE_NAMESPACE_BEGIN

class OpenGLContext :public Context
{
public:
	OpenGLContext(GLFWwindow* glfwWindow);
	virtual void Init() override;
	virtual void SwapBuffers()override;
private:
	GLFWwindow* m_pGLFWWindow;
};

SAND_TABLE_NAMESPACE_END