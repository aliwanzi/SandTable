#pragma once
#include "SandTable/Core/Core.h"
#include "SandTable/Core/Window.h"
#include "SandTable/Core/LayerStack.h"
#include "SandTable/ImGui/ImGuiLayer.h"

namespace SandTable
{
	class SAND_TABLE_API Application
	{
	public:
		//For Application
		static void CreateApplication();

		virtual ~Application();
		void Run();
		void OnEvent(Event& event);
		void PushLayer(std::shared_ptr<Layer> spLayer);
		void PushOverlay(std::shared_ptr<Layer> spLayer);
		int GetWindowWidth() const;
		int GetWindowHeight() const;
		const std::unique_ptr<Window>& GetWindow()const;

		static std::shared_ptr<Application> GetApplication();
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

		std::unique_ptr<Window> m_upWindow;
		std::shared_ptr<LayerStack> m_spLayerStack;
		std::shared_ptr<ImGuiLayer> m_spImGuiLayer;

		static std::shared_ptr<Application> m_spApplication;
	};
}


