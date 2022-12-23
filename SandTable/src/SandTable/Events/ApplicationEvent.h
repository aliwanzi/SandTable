#pragma once
#include "SandTable/Events/Event.h"

SAND_TABLE_NAMESPACE_BEGIN

class WindowResizedEvent :public Event
{
public:
	WindowResizedEvent(unsigned int uiWidth, unsigned int uiHeight);
	unsigned int GetWidth()const;
	unsigned int GetHeight()const;
	std::string ToString() const override;

	// 通过 Event 继承
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
	// 通过 Event 继承
	EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppTickEvent : public Event
{
public:
	AppTickEvent() = default;
	// 通过 Event 继承
	EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppUpdateEvent : public Event
{
public:
	AppUpdateEvent() = default;
	// 通过 Event 继承
	EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppRenderEvent : public Event
{
public:
	AppRenderEvent() = default;
	// 通过 Event 继承
	EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

SAND_TABLE_NAMESPACE_END