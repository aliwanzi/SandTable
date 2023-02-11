#include "pch.h"
#define GL_GLEXT_PROTOTYPES
#include "GL/gl3w.h"
#include "WindowsWindow.h"
#include "PlatForm/OpenGL/OpenGLContext.h"
#include "SandTable/Debug/Instrumentor.h"

SAND_TABLE_NAMESPACE_BEGIN

Ref<Context> WindowsWindow::m_spContext = nullptr;
namespace
{
	static void GLFWErrorCallback(int error, const char* description)
	{
		LOG_DEV_ERROR("GLFW Error ({0}): {1}", error, description);
	}

}

WindowsWindow::WindowsWindow(const WindowProps& m_WindowProps):Window(m_WindowProps)
{
	SAND_TABLE_PROFILE_FUNCTION();
	Init();
}

WindowsWindow::~WindowsWindow()
{
	Shutdown();
}

void WindowsWindow::OnUpdate()
{
	m_spContext->SwapBuffers();
	glfwPollEvents();
}

inline unsigned int WindowsWindow::GetWindowWidth() const
{
	return m_windowCallBack.Width;
}

inline unsigned int WindowsWindow::GetWindowHeight() const
{
	return m_windowCallBack.Height;
}

inline void WindowsWindow::SetEventCallback(const EventCallbackFn& eventCallBack)
{
	m_windowCallBack.EventCallback = eventCallBack;
}

void WindowsWindow::SetVSync(bool bEnabled)
{
	SAND_TABLE_PROFILE_FUNCTION();
	bEnabled ? glfwSwapInterval(1) : glfwSwapInterval(0);
	m_windowCallBack.VSync = bEnabled;
}

bool WindowsWindow::IsVSync() const
{
	return false;
}

void* WindowsWindow::GetNativeWindow() const
{
	return m_pGLFWWindow;
}

void WindowsWindow::Init()
{
	SAND_TABLE_PROFILE_FUNCTION();

	LOG_DEV_INFO("Creating window {0} ({1}, {2})",
		m_WindowProps.Title, m_WindowProps.Width, m_WindowProps.Height);
	bool bInit = glfwInit();
	SAND_TABLE_ASSERT(bInit, "Failed to initialize GLFW");

	m_windowCallBack.Width = m_WindowProps.Width;
	m_windowCallBack.Height = m_WindowProps.Height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_WindowProps.MajorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_WindowProps.MinorVersion);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef SAND_TABLE_PLATFORM_APPLE
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
	#endif
	glfwWindowHint(GLFW_SAMPLES, m_WindowProps.Samples);

	{
		SAND_TABLE_PROFILE_SCOPE("glfw Create Window")
		m_pGLFWWindow = glfwCreateWindow(m_WindowProps.Width,
			m_WindowProps.Height, m_WindowProps.Title.c_str(), nullptr, nullptr);
	}

	SAND_TABLE_ASSERT(m_pGLFWWindow, "Failed to creat GLFW window");
	m_spContext = Ref<OpenGLContext>(new OpenGLContext(m_pGLFWWindow));
	m_spContext->Init();

	glfwSetErrorCallback(GLFWErrorCallback);
	glfwSetWindowUserPointer(m_pGLFWWindow, &m_windowCallBack);
	SetVSync(true);

	//Set GLFW callbacks
	glfwSetWindowSizeCallback(m_pGLFWWindow, [](GLFWwindow* window, int width, int height)
		{
			auto& pWindowCallBack = *(WindowCallBack*)glfwGetWindowUserPointer(window);
			pWindowCallBack.Width = width;
			pWindowCallBack.Height = height;

			WindowResizedEvent event(width, height);
			LOG_DEV_WARN("window resize {0},{1}", width, height);
			pWindowCallBack.EventCallback(event);
		});

	glfwSetWindowCloseCallback(m_pGLFWWindow, [](GLFWwindow* window)
		{
			auto& pWindowCallBack = *(WindowCallBack*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			pWindowCallBack.EventCallback(event);
		});

	glfwSetKeyCallback(m_pGLFWWindow, [](GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
		{
			auto& pWindowCallBack = *(WindowCallBack*)glfwGetWindowUserPointer(window);
			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, false);
				pWindowCallBack.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				pWindowCallBack.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, true);
				pWindowCallBack.EventCallback(event);
				break;
			}
			}
		});

	glfwSetCharCallback(m_pGLFWWindow, [](GLFWwindow* window, unsigned int keycode)
		{
			auto& pWindowCallBack = *(WindowCallBack*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			pWindowCallBack.EventCallback(event);
		});

	glfwSetMouseButtonCallback(m_pGLFWWindow, [](GLFWwindow* window, int button, int action, int mods)
		{
			auto& pWindowCallBack = *(WindowCallBack*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				pWindowCallBack.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				pWindowCallBack.EventCallback(event);
				break;
			}
			}
		});

	glfwSetScrollCallback(m_pGLFWWindow, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			auto& pWindowCallBack = *(WindowCallBack*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
			pWindowCallBack.EventCallback(event);
		});

	glfwSetCursorPosCallback(m_pGLFWWindow, [](GLFWwindow* window, double xPos, double yPos)
		{
			auto& pWindowCallBack = *(WindowCallBack*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
			pWindowCallBack.EventCallback(event);
		});
}

void WindowsWindow::Shutdown()
{
	glfwDestroyWindow(m_pGLFWWindow);
}

SAND_TABLE_NAMESPACE_END