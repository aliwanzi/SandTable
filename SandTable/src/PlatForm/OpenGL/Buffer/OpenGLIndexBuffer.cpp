#include "pch.h"
#include "OpenGLIndexBuffer.h"
#include "GL/gl3w.h"

SAND_TABLE_NAMESPACE_BEGIN
OpenGLIndexBuffer::OpenGLIndexBuffer(const std::vector<int>& vecIndex) :
	IndexBuffer(vecIndex)
{
	glGenBuffers(1, &m_uiRenderID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiRenderID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vecIndex.size() * sizeof(int), &m_vecIndex[0], GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	glDeleteBuffers(1, &m_uiRenderID);
}

void OpenGLIndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiRenderID);
}

void OpenGLIndexBuffer::UnBind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

SAND_TABLE_NAMESPACE_END