#pragma once
#include "Core.h"
#include "Window.h"

namespace SandTable
{
	class SAND_TABLE_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);
		static std::shared_ptr<Application> CreateApplication();
		
	private:
		bool OnWindowClosed(WindowCloseEvent& e);
	private:
		std::unique_ptr<Window> m_upWindow;
		bool m_bRunning;
	};
}


