#pragma once
#include "SandTable/Events/Event.h"
#include "SandTable/Core/MouseCode.h"

namespace SandTable
{
	class MouseMovedEvent :public Event
	{
	public:
		MouseMovedEvent(float fMouseX, float fMouseY);
		float GetMouseX() const;
		float GetMouseY() const;
		std::string ToString()const override;
		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_fMouseX;
		float m_fMouseY;
	};

	class MouseScrolledEvent :public Event
	{
	public:
		MouseScrolledEvent(float fXOffset, float fYOffset);
		float GetXOffset()const;
		float GetYOffset()const;
		std::string ToString()const override;
		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_fXOffset;
		float m_fYOffset;
	};

	class MouseButtonEvent :public Event
	{
	public:
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton);
		MouseCode GetMouseButton()const;
	protected:
		MouseButtonEvent(MouseCode uiMouseCode);
		MouseCode m_uiMouseCode;
	};

	class MouseButtonPressedEvent :public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(MouseCode uiMouseCode);
		std::string ToString()const override;
		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent :public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(MouseCode uiMouseCode);
		std::string ToString()const override;
		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}


