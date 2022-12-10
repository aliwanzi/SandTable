#include "pch.h"
#include "Application.h"
#include "SandTable/Events/ApplicationEvent.h"

namespace SandTable
{
#define BIND_EVENT_FUN(x) std::bind(&x, this, std::placeholders::_1)
	Application::Application():m_bRunning(true)
	{
		m_upWindow = std::unique_ptr<Window>(Window::Create());
		m_upWindow->SetEventCallback(BIND_EVENT_FUN(Application::OnEvent));
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_bRunning)
		{
			m_upWindow->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUN(Application::OnWindowClosed));
		LOG_DEV_INFO(e);
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_bRunning = false;
		return true;
	}
}
