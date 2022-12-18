#pragma once
#include "SandTable/Core/Core.h"
#include "SandTable/Core/Window.h"
#include "SandTable/Core/LayerStack.h"
#include "SandTable/ImGui/ImGuiLayer.h"

SAND_TABLE_NAMESPACE_BEGIN

class Application
{
public:
	//For Application
	static void CreateApplication();

	virtual ~Application();
	void Run();
	void OnEvent(Event& event);
	void PushLayer(Ref<Layer> spLayer);
	void PushOverlay(Ref<Layer> spLayer);
	int GetWindowWidth() const;
	int GetWindowHeight() const;
	const Scope<Window>& GetWindow()const;

	static Ref<Application> GetApplication();
private:
	void Init();
	Application();
	Application(Application&) = delete;
	Application& operator=(const Application&) = delete;
private:
	bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event);
	bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event);
	bool OnMouseMovedEvent(MouseMovedEvent& event);
	bool OnMouseScrolledEvent(MouseScrolledEvent& event);

	bool OnKeyPressedEvent(KeyPressedEvent& event);
	bool OnKeyReleasedEvent(KeyReleasedEvent& event);
	bool OnKyeTypedEvent(KeyEvent& event);

	bool OnWindowResizedEvent(WindowResizedEvent& event);
	bool OnWindowClosedEvent(WindowCloseEvent& e);
private:
	bool m_bRunning;
	float m_fLastFrameTime;

	Scope<Window> m_upWindow;
	Ref<LayerStack> m_spLayerStack;
	Ref<ImGuiLayer> m_spImGuiLayer;

	static Ref<Application> m_spApplication;
};

SAND_TABLE_NAMESPACE_END