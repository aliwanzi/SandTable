#include "pch.h"
#include "Application.h"
#include "SandTable/Events/ApplicationEvent.h"
#include "GL/gl3w.h"

namespace SandTable
{
#define BIND_EVENT_FUN(x) std::bind(&x, this, std::placeholders::_1)
	Application::Application():m_bRunning(true)
	{
		m_upWindow = std::unique_ptr<Window>(Window::Create());
		m_upWindow->SetEventCallback(BIND_EVENT_FUN(Application::OnEvent));
		m_spLayerStack = std::make_unique<LayerStack>();
		GLuint id;
		glGenVertexArrays(1, &id);
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
	}

	void Application::PushOverlay(std::shared_ptr<Layer> spLayer)
	{
		m_spLayerStack->PushOverlay(spLayer);
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_bRunning = false;
		return true;
	}
}
