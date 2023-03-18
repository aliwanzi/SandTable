#pragma once

SAND_TABLE_NAMESPACE_BEGIN

using MouseCode = std::uint32_t;
namespace Mouse
{
	enum :MouseCode
	{
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	};
}

enum class CursorMode
{
	Normal = 0,
	Hidden = 1,
	Locked = 2
};

SAND_TABLE_NAMESPACE_END