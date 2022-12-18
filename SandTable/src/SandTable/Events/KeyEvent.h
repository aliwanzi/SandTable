#pragma once
#include "SandTable/Events/Event.h"
#include "SandTable/Core/KeyCode.h"

namespace SandTable
{
	class KeyEvent:public Event
	{
	public :
		KeyCode GetKeyCode()const;
		EVENT_CLASS_CATEGORY(EventCategoryKeyBoard| EventCategoryInput)
	protected:
		KeyEvent(KeyCode uiKeyCode);
		KeyCode m_uiKeyCode;
	};

	class KeyPressedEvent :public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode uiKeyCode, bool bRepeat = false);
		bool IsRepeat()const;
		std::string ToString()const override;
		EVENT_CLASS_TYPE(KeyPressed)
	private:
		bool m_bRepeat;
	};

	class KeyReleasedEvent :public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode uiKeyCode);
		std::string ToString()const override;
		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent :public KeyEvent
	{
	public:
		KeyTypedEvent(KeyCode uiKeyCode);
		std::string ToString()const override;
		EVENT_CLASS_TYPE(KeyTyped)
	};
}


