#include "pch.h"
#include "OpenGLUniformBuffer.h"
#include "GL/gl3w.h"

SAND_TABLE_NAMESPACE_BEGIN

OpenGLUniformBuffer::OpenGLUniformBuffer(unsigned int uiSize, unsigned int uiBinding)
{
	glGenBuffers(1, &m_uiRenderID);

	glBindBuffer(GL_UNIFORM_BUFFER, m_uiRenderID);
	glBufferData(GL_UNIFORM_BUFFER, uiSize, NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, uiBinding, m_uiRenderID, 0, uiSize);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

OpenGLUniformBuffer::~OpenGLUniformBuffer()
{
	glDeleteBuffers(1, &m_uiRenderID);
}

void OpenGLUniformBuffer::SetData(const void* data, unsigned int uiSize, unsigned int uiOffset)
{
	Bind();
	glBufferSubData(GL_UNIFORM_BUFFER, uiOffset, uiSize, data);
	UnBind();
}

void OpenGLUniformBuffer::Bind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_uiRenderID);
}

void OpenGLUniformBuffer::UnBind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

SAND_TABLE_NAMESPACE_END
