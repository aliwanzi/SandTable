#pragma once
#include "SandTable/Render/Buffer/VertexBuffer.h"
#include "SandTable/Render/Buffer/IndexBuffer.h"

SAND_TABLE_NAMESPACE_BEGIN

class VertexArray
{
public:
	virtual ~VertexArray() = default;
	virtual void Bind()const = 0;
	virtual void UnBind() const = 0;
	virtual void AddVertexBuffer(const Ref<Buffer>& spBuffer) = 0;
	virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() const = 0;
	virtual void SetIndexBuffer(const Ref<Buffer>& spBuffer) = 0;
	virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;
	static Ref<VertexArray> Create();
protected:
	VertexArray();
private:
	VertexArray(VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;
protected:
	unsigned int m_uiRenderID;
};

SAND_TABLE_NAMESPACE_END