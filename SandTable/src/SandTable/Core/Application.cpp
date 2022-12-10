#include "pch.h"
#include "Application.h"
#include "SandTable/Events/ApplicationEvent.h"
#include "GL/gl3w.h"

namespace SandTable
{
#define BIND_EVENT_FUN(x) std::bind(&x, this, std::placeholders::_1)

	std::shared_ptr<Application> Application::m_spApplication = nullptr;

	Application::Application():m_bRunning(true)
	{
		m_upWindow = std::unique_ptr<Window>(Window::Create());
		m_upWindow->SetEventCallback(BIND_EVENT_FUN(Application::OnEvent));
		m_spLayerStack = std::make_unique<LayerStack>();
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_bRunning)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_upWindow->OnUpdate();
			const auto& listLayers = m_spLayerStack->GetLayers();
			for (auto iter = listLayers.begin(); iter != listLayers.end(); iter++)
			{
				(*iter)->OnUpdate();
			}
		}		
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUN(Application::OnWindowClosed));
		const auto& listLayers = m_spLayerStack->GetLayers();
		for (auto iter = listLayers.begin(); iter != listLayers.end(); iter++)
		{
			(*iter)->OnEvent(e);
			if (e.Handle())
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

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_bRunning = false;
		return true;
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
