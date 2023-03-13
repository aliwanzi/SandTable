#pragma once
#include "SandTable/Core/Window.h"
#include "SandTable/Core/LayerStack.h"
#include "SandTable/ImGui/ImGuiLayer.h"

SAND_TABLE_NAMESPACE_BEGIN

struct ApplicationCommandLineArgs
{
	int Count = 0;
	char** Args = nullptr;

	const char* operator[](int index) const
	{
		SAND_TABLE_ASSERT(index < Count,"index is less than count");
		return Args[index];
	}
};

struct ApplicationSpecification
{
	std::string Name = "SandBox Application";
	std::string WorkingDirectory;
	ApplicationCommandLineArgs CommandLineArgs;
};

class Application
{
public:
	//For Application
	static void CreateApplication(int argc, char** argv);

	virtual ~Application();
	void Run();
	void OnEvent(Event& event);
	void BlockEvents(bool bBlock);
	void PushLayer(Ref<Layer> spLayer);
	void PushOverlay(Ref<Layer> spLayer);
	void Close();
	int GetWindowWidth() const;
	int GetWindowHeight() const;
	const Ref<Window>& GetWindow()const;

	const Ref<ApplicationSpecification>& GetApplicationSpecification()const;
	static Ref<Application> GetApplication(Ref<ApplicationSpecification> spApplicationSpecification = nullptr);

	void SubmitToMainThreadQueue(const std::function<void()>& func);
	void ExecuteMainThreadQueue();
private:
	void Init();
	Application() = default;
	Application(Ref<ApplicationSpecification> spApplicationSpecification);
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
	bool m_bMinimized;
	float m_fLastFrameTime;

	Ref<Window> m_spWindow;
	Ref<LayerStack> m_spLayerStack;
	Ref<ImGuiLayer> m_spImGuiLayer;
	Ref<ApplicationSpecification> m_spApplicationSpecification;

	static Ref<Application> m_spApplication;

	std::vector<std::function<void()>> m_vecMainThreadQueue;
	std::mutex m_mutexMainThreadQueue;
};

SAND_TABLE_NAMESPACE_END