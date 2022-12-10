#pragma once
#include "Core.h"
#include "Window.h"
#include "LayerStack.h"

namespace SandTable
{
	class SAND_TABLE_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);
		void PushLayer(std::shared_ptr<Layer> spLayer);
		void PushOverlay(std::shared_ptr<Layer> spLayer);

		static std::shared_ptr<Application> CreateApplication();
		
	private:
		bool OnWindowClosed(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_upWindow;
		bool m_bRunning;
		std::shared_ptr<LayerStack> m_spLayerStack;
	};
}


