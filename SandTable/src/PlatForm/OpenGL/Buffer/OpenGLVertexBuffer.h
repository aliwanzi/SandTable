#pragma once
#include "SandTable/Render/Buffer/VertexBuffer.h"

namespace SandTable
{
	class OpenGLVertexBuffer :public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const std::vector<float>& vecVertics,
			const std::shared_ptr<VertexBufferLayout>& spVertexBufferLayout);
		~OpenGLVertexBuffer();
		virtual void Bind() const override;
		virtual void UnBind() const override;
	};
}


