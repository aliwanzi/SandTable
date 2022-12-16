#pragma once
#include "SandTable/Render/Context.h"

struct GLFWwindow;
namespace SandTable
{
	class OpenGLContext :public Context
	{
	public:
		OpenGLContext(GLFWwindow* glfwWindow);
		virtual void Init() override;
		virtual void SwapBuffers()override;
	private:
		GLFWwindow* m_pGLFWWindow;
	};
}