#include "SandTable/Core/Window.h"
#include <GLFW/glfw3.h>
#include "SandTable/Render/Context.h"

SAND_TABLE_NAMESPACE_BEGIN

class WindowsWindow :public Window
{
public:
	WindowsWindow(const WindowProps& windowPorps);
	virtual ~WindowsWindow();

	void OnUpdate() override;

	inline unsigned int GetWindowWidth() const override;
	inline unsigned int GetWindowHeight() const override;

	// Window attributes
	inline void SetEventCallback(const EventCallbackFn& eventCallBack) override;
	void SetVSync(bool bEnabled) override;
	bool IsVSync() const override;
	void* GetNativeWindow()const override;
private:
	virtual void Init(const WindowProps& windowPorps);
	virtual void Shutdown();
private:
	static Ref<Context> m_spContext;
	GLFWwindow* m_pGLFWWindow;

	struct WindowCallBack
	{
		std::string Title;
		int Width;
		int Height;
		bool VSync;

		EventCallbackFn EventCallback;
	};

	WindowCallBack m_windowCallBack;
};

SAND_TABLE_NAMESPACE_END