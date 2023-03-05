#include "pch.h"
#include "OpenGLContext.h"
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include "SandTable/Statics/Instrumentor.h"

SAND_TABLE_NAMESPACE_BEGIN

OpenGLContext::OpenGLContext(GLFWwindow* glfwWindow) :
	m_pGLFWWindow(glfwWindow)
{
	SAND_TABLE_ASSERT(m_pGLFWWindow, "GLFW window in OpenGLContext is null");
}

void OpenGLContext::Init()
{
	SAND_TABLE_PROFILE_FUNCTION();
	glfwMakeContextCurrent(m_pGLFWWindow);

	int bInit = gl3wInit();
	SAND_TABLE_ASSERT(!bInit, "Failed to initialize GL3W");

	LOG_DEV_INFO("{0}", sizeof(float));
	LOG_DEV_INFO("OpenGL Info:");
	LOG_DEV_INFO("Vendor: {0}", glGetString(GL_VENDOR));
	LOG_DEV_INFO("Renderer:{0}", glGetString(GL_RENDERER));
	LOG_DEV_INFO("Version: {0}", glGetString(GL_VERSION));
}

void OpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(m_pGLFWWindow);
}

SAND_TABLE_NAMESPACE_END