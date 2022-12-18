#pragma once
#include "SandTable/Core/Logger.h"

namespace SandTable
{
	class Buffer
	{
	public:
		Buffer():m_uiRenderID(0) {}
		virtual ~Buffer() = default;
		virtual void Bind()const {};
		virtual void UnBind()const {};
	protected:
		unsigned int m_uiRenderID;
	};
}