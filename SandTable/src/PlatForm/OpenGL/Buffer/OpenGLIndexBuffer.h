#pragma once
#include "SandTable/Render/Buffer/IndexBuffer.h"

SAND_TABLE_NAMESPACE_BEGIN

class OpenGLIndexBuffer :public IndexBuffer
{
public:
	OpenGLIndexBuffer(const std::vector<int>& vecIndex);
	~OpenGLIndexBuffer();
	virtual void Bind() const override;
	virtual void UnBind() const override;
};

SAND_TABLE_NAMESPACE_END