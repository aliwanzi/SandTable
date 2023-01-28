#pragma once
#include "SandTable/Render/Buffer/UniformBuffer.h"
SAND_TABLE_NAMESPACE_BEGIN

class OpenGLUniformBuffer :public UniformBuffer
{
public:
	OpenGLUniformBuffer(unsigned int uiSize, unsigned int uiBinding);
	virtual ~OpenGLUniformBuffer();

	virtual void SetData(const void* data, unsigned int uiSize, unsigned int uiOffset = 0) override;

	virtual void Bind() const override;
	virtual void UnBind() const override;
};

SAND_TABLE_NAMESPACE_END

