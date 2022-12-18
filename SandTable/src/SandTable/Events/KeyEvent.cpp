#include "pch.h"
#include "KeyEvent.h"

SAND_TABLE_NAMESPACE_BEGIN

KeyCode KeyEvent::GetKeyCode() const
{
	return m_uiKeyCode;
}
KeyEvent::KeyEvent(KeyCode uiKeyCode) :
	m_uiKeyCode(uiKeyCode)
{

}

KeyPressedEvent::KeyPressedEvent(KeyCode uiKeyCode, bool bRepeat) :
	KeyEvent(uiKeyCode), m_bRepeat(bRepeat)
{
}
bool KeyPressedEvent::IsRepeat() const
{
	return m_bRepeat;
}
std::string KeyPressedEvent::ToString() const
{
	std::stringstream ss;
	ss << "KeyPressed: " << m_uiKeyCode << " (repeat = " << m_bRepeat << ")";
	return ss.str();
}

KeyReleasedEvent::KeyReleasedEvent(KeyCode uiKeyCode) :
	KeyEvent(uiKeyCode)
{
}

std::string KeyReleasedEvent::ToString() const
{
	std::stringstream ss;
	ss << "KeyReleased: " << m_uiKeyCode;
	return ss.str();
}

KeyTypedEvent::KeyTypedEvent(KeyCode uiKeyCode) :
	KeyEvent(uiKeyCode)
{
}

std::string KeyTypedEvent::ToString() const
{
	std::stringstream ss;
	ss << "KeyTyped: " << m_uiKeyCode;
	return ss.str();
}

SAND_TABLE_NAMESPACE_END
