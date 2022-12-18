#pragma once
#include "SandTable/Core/Logger.h"

namespace SandTable
{
	class Context
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}