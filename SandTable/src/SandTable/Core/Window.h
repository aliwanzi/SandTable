#pragma once
#include <functional>

#include "SandTable/Events/MouseEvent.h"
#include "SandTable/Events/KeyEvent.h"
#include "SandTable/Events/ApplicationEvent.h"

namespace SandTable
{
	struct WindowProps
	{
		std::string Title;
		int Width;
		int Height;
		int MajorVersion;
		int MinorVersion;
		int Samples;
		WindowProps(const std::string& sTitle = "SandTable", int iWidth = 1280, int iHeight = 720, int Samples = 0)
			: Title(sTitle),
			Width(iWidth),
			Height(iHeight),
			MajorVersion(4),
			MinorVersion(3),
			Samples(Samples)
		{
		}
	};

	class SAND_TABLE_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWindowWidth() const = 0;
		virtual unsigned int GetWindowHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& windowPorps = WindowProps());
	};
}
