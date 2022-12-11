#include "pch.h"
#include "Application.h"
#include "SandTable/Events/ApplicationEvent.h"
#include "GLFW/glfw3.h"

namespace SandTable
{
	std::shared_ptr<Application> Application::m_spApplication = nullptr;

	Application::Application():m_bRunning(true)
	{
		m_upWindow = std::unique_ptr<Window>(Window::Create());
		m_upWindow->SetEventCallback(BIND_EVENT_FUN(Application::OnEvent));
		m_spLayerStack = std::make_unique<LayerStack>();
	}

	bool Application::OnMouseButtonPressedEvent(MouseButtonPressedEvent& event)
	{
		return false;
	}

	bool Application::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event)
	{
		return false;
	}

	bool Application::OnMouseMovedEvent(MouseMovedEvent& event)
	{
		return false;
	}

	bool Application::OnMouseScrolledEvent(MouseScrolledEvent& event)
	{
		return false;
	}

	bool Application::OnKeyPressedEvent(KeyPressedEvent& event)
	{
		if (event.GetKeyCode() == GLFW_KEY_ESCAPE)
		{
			m_bRunning = false;
		}
		return true;
	}

	bool Application::OnKeyReleasedEvent(KeyReleasedEvent& event)
	{
		return false;
	}

	bool Application::OnKyeTypedEvent(KeyEvent& event)
	{
		return false;
	}

	bool Application::OnWindowResizedEvent(WindowResizedEvent& event)
	{
		return false;
	}

	bool Application::OnWindowClosedEvent(WindowCloseEvent& e)
	{
		m_bRunning = false;
		return true;
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_bRunning)
		{
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			const auto& listLayers = m_spLayerStack->GetLayers();
			for (auto iter = listLayers.begin(); iter != listLayers.end(); iter++)
			{
				(*iter)->OnUpdate();
			}
			m_upWindow->OnUpdate();
		}
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUN(Application::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FUN(Application::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FUN(Application::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FUN(Application::OnMouseScrolledEvent));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUN(Application::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FUN(Application::OnKeyReleasedEvent));
		dispatcher.Dispatch<WindowResizedEvent>(BIND_EVENT_FUN(Application::OnWindowResizedEvent));

		const auto& listLayers = m_spLayerStack->GetLayers();
		for (auto iter = listLayers.begin(); iter != listLayers.end(); iter++)
		{
			(*iter)->OnEvent(event);
			if (event.Handle())
			{
				break;
			}
		}
	}

	void Application::PushLayer(std::shared_ptr<Layer> spLayer)
	{
		m_spLayerStack->PushLayer(spLayer);
		spLayer->OnAttach();
	}

	void Application::PushOverlay(std::shared_ptr<Layer> spLayer)
	{
		m_spLayerStack->PushOverlay(spLayer);
		spLayer->OnAttach();
	}

	int Application::GetWindowWidth() const
	{
		return m_upWindow->GetWindowWidth();
	}

	int Application::GetWindowHeight() const
	{
		return m_upWindow->GetWindowHeight();
	}

	const std::unique_ptr<Window>& Application::GetWindow() const
	{
		return m_upWindow;
	}

	std::shared_ptr<Application> Application::GetApplication()
	{
		if (!m_spApplication)
		{
			m_spApplication = std::shared_ptr<Application>(new Application());
		}
		return m_spApplication;
	}
}
