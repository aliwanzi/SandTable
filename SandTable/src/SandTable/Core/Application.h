#pragma once
#include "Core.h"
#include "Window.h"
#include "LayerStack.h"

namespace SandTable
{
	class SAND_TABLE_API Application
	{
	public:
		//For Application
		static void CreateApplication();

		virtual ~Application();
		void Run();
		void OnEvent(Event& e);
		void PushLayer(std::shared_ptr<Layer> spLayer);
		void PushOverlay(std::shared_ptr<Layer> spLayer);
		int GetWindowWidth() const;
		int GetWindowHeight() const;

		static std::shared_ptr<Application> GetApplication();

		
	private:
		Application();
		Application(Application&) = delete;
		Application& operator=(const Application&) = delete;
		bool OnWindowClosed(WindowCloseEvent& e);
	private:
		std::unique_ptr<Window> m_upWindow;
		bool m_bRunning;

		std::shared_ptr<LayerStack> m_spLayerStack;
		static std::shared_ptr<Application> m_spApplication;
	};
}


