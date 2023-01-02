#include "pch.h"
#include "Application.h"
#include "SandTable/Events/ApplicationEvent.h"
#include "SandTable/Core/Input.h"
#include "SandTable/Render/Render.h"
#include "SandTable/Render/Render2D.h"
#include "SandTable/Debug/Instrumentor.h"

SAND_TABLE_NAMESPACE_BEGIN

Ref<Application> Application::m_spApplication = nullptr;

Application::Application() :m_bRunning(true), m_fLastFrameTime(0.f)
{
	SAND_TABLE_PROFILE_FUNCTION();
	m_spWindow = Window::Create();
	m_spWindow->SetEventCallback(BIND_EVENT_FUN(Application::OnEvent));
}

void Application::Init()
{
	SAND_TABLE_PROFILE_FUNCTION();
	Render::Init();
	Render2D::Init();
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
	SAND_TABLE_PROFILE_FUNCTION();
	while (m_bRunning)
	{
		float fTime = TimeStep::GetTime();
		TimeStep timeStep = fTime - m_fLastFrameTime;
		m_fLastFrameTime = fTime;

		const auto& listLayers = m_spLayerStack->GetLayers();

		if (!m_bMinimized)
		{
			{
				SAND_TABLE_PROFILE_SCOPE("Layer OnUpdate")
				for (auto iter = listLayers.begin(); iter != listLayers.end(); iter++)
				{
					(*iter)->OnUpdate(timeStep);
				}
			}

			m_spImGuiLayer->BeginNewFrame();
			{
				SAND_TABLE_PROFILE_SCOPE("Layer OnImGuiRender")
				for (auto iter = listLayers.begin(); iter != listLayers.end(); iter++)
				{
					(*iter)->OnImGuiRender();
				}
			}

			m_spImGuiLayer->EndNewFrame();
		}
		m_spWindow->OnUpdate();
	}
}

void Application::OnEvent(Event& e)
{
	//SAND_TABLE_PROFILE_FUNCTION();
	EventDispatcher dispatcher(e);
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
	for (auto iter = listLayers.rbegin(); iter != listLayers.rend(); iter++)
	{
		if (e.Handle)
		{
			break;
		}
		(*iter)->OnEvent(e);
	}
}

void Application::BlockEvents(bool bBlock)
{
	m_spImGuiLayer->BlockEvents(bBlock);
}

void Application::PushLayer(Ref<Layer> spLayer)
{
	SAND_TABLE_PROFILE_FUNCTION();

	m_spLayerStack->PushLayer(spLayer);
	spLayer->OnAttach();
}

void Application::PushOverlay(Ref<Layer> spLayer)
{
	SAND_TABLE_PROFILE_FUNCTION();

	m_spLayerStack->PushOverlay(spLayer);
	spLayer->OnAttach();
}

void Application::Close()
{
	m_bRunning = false;
}

int Application::GetWindowWidth() const
{
	return m_spWindow->GetWindowWidth();
}

int Application::GetWindowHeight() const
{
	return m_spWindow->GetWindowHeight();
}

const Ref<Window>& Application::GetWindow() const
{
	return m_spWindow;
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