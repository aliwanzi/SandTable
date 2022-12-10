#include "pch.h"
#include "ApplicationEvent.h"

namespace SandTable
{
	WindowResizeEvent::WindowResizeEvent(unsigned int uiWidth, unsigned int uiHeight) :
		m_uiWidth(uiWidth), m_uiHeight(uiHeight)
	{
	}
	inline unsigned int WindowResizeEvent::GetWidth() const
	{
		return m_uiWidth;
	}
	inline unsigned int WindowResizeEvent::GetHeight() const
	{
		return m_uiHeight;
	}
	std::string WindowResizeEvent::ToString() const
	{
		std::stringstream ss;
		ss << "WindowResizeEvent: " << m_uiWidth << " , " << m_uiHeight;
		return ss.str();
	}
}
