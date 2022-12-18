#pragma once
#include "SandTable/Render/Buffer/VertexBuffer.h"
#include "SandTable/Render/Buffer/IndexBuffer.h"

namespace SandTable
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;
		virtual void Bind()const = 0;
		virtual void UnBind() const = 0;
		virtual void AddVertexBuffer(const std::shared_ptr<Buffer>& spBuffer) = 0;
		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffer() const = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<Buffer>& spBuffer) = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;
		static std::shared_ptr<VertexArray> Create();
	protected:
		VertexArray();
	private:
		VertexArray(VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;
	protected:
		unsigned int m_uiRenderID;
	};
}


