#pragma once
#include "SandTable/Render/VertexArray.h"

namespace SandTable
{
	class OpenGLVertexArray :public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();
		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual void AddVertexBuffer(const std::shared_ptr<Buffer>& spBuffer) override;
		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffer() const override;
		virtual void SetIndexBuffer(const std::shared_ptr<Buffer>& spBuffer) override;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override;

	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_vecVertexBuffer;
		std::shared_ptr<IndexBuffer> m_spIndexBuffer;
	};
}