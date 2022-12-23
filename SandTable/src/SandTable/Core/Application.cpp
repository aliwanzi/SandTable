#include "pch.h"
#include "Application.h"
#include "SandTable/Events/ApplicationEvent.h"
#include "SandTable/Core/Input.h"
#include "SandTable/Render/Render.h"

SAND_TABLE_NAMESPACE_BEGIN

Ref<Application> Application::m_spApplication = nullptr;

Application::Application() :m_bRunning(true), m_fLastFrameTime(0.f)
{
	m_upWindow = Scope<Window>(Window::Create());
	m_upWindow->SetEventCallback(BIND_EVENT_FUN(Application::OnEvent));
}

void Application::Init()
{
	Render::Init();
	m_spLayerStack = CreateRef<LayerStack>();
	m_spImGuiLayer = CreateRef<ImGuiLayer>();
	PushOverlay(m_spImGuiLayer);
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
	if (event.GetKeyCode() == Key::Escape)
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
	if (event.GetWidth() == 0 || event.GetHeight() == 0)
	{
		m_bMinimized = true;
		return false;
	}
	m_bMinimized = false;
	Render::OnWindowResize(event.GetWidth(), event.GetHeight());
	return false;
}

bool Application::OnWindowClosedEvent(WindowCloseEvent& /*event*/)
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
		float fTime = TimeStep::GetTime();
		TimeStep timeStep = fTime - m_fLastFrameTime;
		m_fLastFrameTime = fTime;

		const auto& listLayers = m_spLayerStack->GetLayers();

		if (!m_bMinimized)
		{
			for (auto iter = listLayers.begin(); iter != listLayers.end(); iter++)
			{
				(*iter)->OnUpdate(timeStep);
			}
		}

		m_spImGuiLayer->BeginNewFrame();
		for (auto iter = listLayers.begin(); iter != listLayers.end(); iter++)
		{
			(*iter)->OnImGuiRender();
		}
		m_spImGuiLayer->EndNewFrame();

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
	dispatcher.Dispatch<KeyTypedEvent>(BIND_EVENT_FUN(Application::OnKyeTypedEvent));
	dispatcher.Dispatch<WindowResizedEvent>(BIND_EVENT_FUN(Application::OnWindowResizedEvent));
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUN(Application::OnWindowClosedEvent));

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

void Application::PushLayer(Ref<Layer> spLayer)
{
	m_spLayerStack->PushLayer(spLayer);
	spLayer->OnAttach();
}

void Application::PushOverlay(Ref<Layer> spLayer)
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

const Scope<Window>& Application::GetWindow() const
{
	return m_upWindow;
}

Ref<Application> Application::GetApplication()
{
	if (m_spApplication == nullptr)
	{
		m_spApplication = Ref<Application>(new Application());
		m_spApplication->Init();
	}
	return m_spApplication;
}

SAND_TABLE_NAMESPACE_END