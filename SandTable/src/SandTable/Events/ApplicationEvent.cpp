#include "pch.h"
#include "ApplicationEvent.h"

SAND_TABLE_NAMESPACE_BEGIN

WindowResizedEvent::WindowResizedEvent(unsigned int uiWidth, unsigned int uiHeight) :
	m_uiWidth(uiWidth), m_uiHeight(uiHeight)
{
}
inline unsigned int WindowResizedEvent::GetWidth() const
{
	return m_uiWidth;
}
inline unsigned int WindowResizedEvent::GetHeight() const
{
	return m_uiHeight;
}
std::string WindowResizedEvent::ToString() const
{
	std::stringstream ss;
	ss << "WindowResizeEvent: " << m_uiWidth << " , " << m_uiHeight;
	return ss.str();
}

SAND_TABLE_NAMESPACE_END