#include "pch.h"
#define GL_GLEXT_PROTOTYPES
#include "GL/gl3w.h"
#include "WindowsWindow.h"

namespace SandTable
{
	namespace
	{
		void OnDebug(GLenum source, GLenum type, GLuint id, GLenum severity,
			GLsizei length, const GLchar* message, GLvoid* userParam)
		{
			switch (severity)
			{
			case GL_DEBUG_SEVERITY_MEDIUM:       LOG_DEV_ERROR(message); return;
			case GL_DEBUG_SEVERITY_LOW:          LOG_DEV_WARN(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_DEV_INFO(message); return;
			}
		}

		bool IsExtensionSupported(const std::string& sExtersionName)
		{
			GLint numExtensions;
			glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

			for (int i = 0; i < numExtensions; i++)
			{
				if (std::string((char const*)glGetStringi(GL_EXTENSIONS, i)) == sExtersionName)
				{
					return true;
				}
			}
			return false;
		}

		static void GLFWErrorCallback(int error, const char* description)
		{
			LOG_DEV_ERROR("GLFW Error ({0}): {1}", error, description);
		}

	}

	Window* Window::Create(const WindowProps& windowPorps)
	{
		return new WindowsWindow(windowPorps);
	}

	WindowsWindow::WindowsWindow(const WindowProps& windowPorps)
	{
		Init(windowPorps);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_pGLFWWindow);
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
		bEnabled ? glfwSwapInterval(1) : glfwSwapInterval(0);
		m_windowCallBack.VSync = bEnabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return false;
	}

	void WindowsWindow::Init(const WindowProps& windowPorps)
	{
		LOG_DEV_INFO("Creating window {0} ({1}, {2})", 
			windowPorps.Title, windowPorps.Width, windowPorps.Height);
		bool bInit = glfwInit();
		SAND_TABLE_ASSERT(bInit, "Failed to initialize GLFW");

		m_windowCallBack.Width = windowPorps.Width;
		m_windowCallBack.Height = windowPorps.Height;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, windowPorps.MajorVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, windowPorps.MinorVersion);

		if (windowPorps.UnionFlags.Debug)
		{
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
		}

		if (windowPorps.UnionFlags.Robust)
		{
			glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_LOSE_CONTEXT_ON_RESET);
		}

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_SAMPLES, windowPorps.Samples);
		glfwWindowHint(GLFW_STEREO, windowPorps.UnionFlags.Stereo ? GL_TRUE : GL_FALSE);

		m_pGLFWWindow = glfwCreateWindow(windowPorps.Width,
			windowPorps.Height, windowPorps.Title.c_str(), nullptr, nullptr);
		SAND_TABLE_ASSERT(m_pGLFWWindow, "Failed to creat GLFW window");

		glfwMakeContextCurrent(m_pGLFWWindow);
		glfwSetWindowUserPointer(m_pGLFWWindow, &m_windowCallBack);
		SetVSync(true);

		//Set GLFW callbacks
		glfwSetWindowSizeCallback(m_pGLFWWindow, [](GLFWwindow* window, int width, int height)
			{
				auto& pWindowCallBack = *(WindowCallBack*)glfwGetWindowUserPointer(window);
				pWindowCallBack.Width = width;
				pWindowCallBack.Height = height;

				WindowResizeEvent event(width, height);
				pWindowCallBack.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_pGLFWWindow, [](GLFWwindow* window)
			{
				auto& pWindowCallBack = *(WindowCallBack*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				pWindowCallBack.EventCallback(event);
			});

		glfwSetKeyCallback(m_pGLFWWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
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

				MouseMoveEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
				pWindowCallBack.EventCallback(event);
			});

		if (!windowPorps.UnionFlags.Cursor)
		{
			glfwSetInputMode(m_pGLFWWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}

		bInit = gl3wInit();
		SAND_TABLE_ASSERT(!bInit, "Failed to initialize GLFW");

		LOG_DEV_INFO("OpenGL Info:");

		LOG_DEV_INFO("Vendor: {0}", glGetString(GL_VENDOR));
		LOG_DEV_INFO("Renderer:{0}", glGetString(GL_RENDERER));
		LOG_DEV_INFO("Version: {0}", glGetString(GL_VERSION));

		if (windowPorps.UnionFlags.Debug)
		{
			if (gl3wIsSupported(4, 3))
			{
				glEnable(GL_DEBUG_OUTPUT);
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
				glDebugMessageCallback((GLDEBUGPROC)OnDebug, this);
				glfwSetErrorCallback(GLFWErrorCallback);
			}
			else if (IsExtensionSupported("GL_ARB_debug_output"))
			{
				glDebugMessageCallbackARB((GLDEBUGPROC)OnDebug, this);
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
			}
		}
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_pGLFWWindow);
	}
}