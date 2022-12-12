#pragma once

#include "Core.h"
#include "KeyCode.h"
#include "MouseCode.h"
#include <glm/glm.hpp>

namespace SandTable
{
	class SAND_TABLE_API Input
	{
	public:
		//Key
		static bool IsKeyPressed(KeyCode keyCode);

		//MouseButton
		static bool IsMouseButtonPressed(KeyCode keyCode);
		static glm::vec2 GetMousePos();
		static float GetMouseXPos();
		static float GetMouseYPos();

	protected:
		virtual bool IsKeyPressedImpl(KeyCode keyCode) = 0;
		virtual bool IsMouseButtonPressedImpl(MouseCode mouseCode) = 0;
		virtual glm::vec2 GetMousePosImpl() = 0;
		virtual float GetMouseXPosImpl() = 0;
		virtual float GetMouseYPosImpl() = 0;

	private:
		static std::shared_ptr<Input> m_spInput;
	};
}