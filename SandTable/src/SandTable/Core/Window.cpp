#include "pch.h"
#include "PlatForm/Windows/WindowsWindow.h"

SAND_TABLE_NAMESPACE_BEGIN

Ref<Window> Window::Create(const WindowProps& windowPorps)
{
#ifdef SAND_TABLE_PLATFORM_WINDOWS
	return CreateRef<WindowsWindow>(windowPorps);
#else
	SAND_TABLE_ASSERT(false, "Unkown platform");
	return nullptr;
#endif // SAND_TABLE_PLATFORM_WINDOW
}

Window::Window(const WindowProps& windowPorps):
	m_WindowProps(windowPorps)
{
}


SAND_TABLE_NAMESPACE_END
