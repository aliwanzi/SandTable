#pragma once
#include "SandTable/Render/Buffer/IndexBuffer.h"

namespace SandTable
{
	class OpenGLIndexBuffer :public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const std::vector<int>& vecIndex);
		~OpenGLIndexBuffer();
		virtual void Bind() const override;
		virtual void UnBind() const override;
	};
}


