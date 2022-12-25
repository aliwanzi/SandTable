#pragma once
#include "SandTable/Render/Buffer/VertexBuffer.h"

SAND_TABLE_NAMESPACE_BEGIN

class OpenGLVertexBuffer :public VertexBuffer
{
public:
	OpenGLVertexBuffer(const std::vector<float>& vecVertics, const Ref<VertexBufferLayout>& spVertexBufferLayout);
	OpenGLVertexBuffer(const std::vector<float>& vecVertics);
	OpenGLVertexBuffer(unsigned int uiVertexSize);
	virtual void SetData(const void* pData, uint32_t uiVertexSize) override;
	~OpenGLVertexBuffer();
	virtual void Bind() const override;
	virtual void UnBind() const override;
};

SAND_TABLE_NAMESPACE_END


