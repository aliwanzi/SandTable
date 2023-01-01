#pragma once
#include "SandTable/Render/Buffer/FrameBuffer.h"

SAND_TABLE_NAMESPACE_BEGIN
class OpenGLFrameBuffer:public FrameBuffer
{
public:
	OpenGLFrameBuffer(const Ref<FrameBufferSpecification>& spFrameBufferSpecification);
	~OpenGLFrameBuffer();
	virtual void Bind() const override;
	virtual void UnBind() const override;
};
SAND_TABLE_NAMESPACE_END

