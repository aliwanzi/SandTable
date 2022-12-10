#pragma once
#include "Event.h"

namespace SandTable
{
	class WindowResizeEvent :public Event
	{
	public:
		WindowResizeEvent(unsigned int uiWidth, unsigned int uiHeight);
		inline unsigned int GetWidth()const;
		inline unsigned int GetHeight()const;
		std::string ToString() const override;

		// ͨ�� Event �̳�
		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_uiWidth;
		unsigned int m_uiHeight;
	};

	class WindowCloseEvent :public Event
	{
	public:
		WindowCloseEvent() = default;
		// ͨ�� Event �̳�
		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;
		// ͨ�� Event �̳�
		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;
		// ͨ�� Event �̳�
		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;
		// ͨ�� Event �̳�
		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}

