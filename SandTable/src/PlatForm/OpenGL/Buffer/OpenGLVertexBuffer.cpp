#include "pch.h"
#include "OpenGLVertexBuffer.h"
#include "GL/gl3w.h"

SAND_TABLE_NAMESPACE_BEGIN

OpenGLVertexBuffer::OpenGLVertexBuffer(const std::vector<float>& vecVertics,
	const Ref<VertexBufferLayout>& spVertexBufferLayout) :
	VertexBuffer(vecVertics, spVertexBufferLayout)
{
	glGenBuffers(1, &m_uiRenderID);
	glBindBuffer(GL_ARRAY_BUFFER, m_uiRenderID);
	glBufferData(GL_ARRAY_BUFFER, m_vecVertex.size(), m_vecVertex.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(const std::vector<float>& vecVertics):
	VertexBuffer(vecVertics)
{
	glGenBuffers(1, &m_uiRenderID);
	glBindBuffer(GL_ARRAY_BUFFER, m_uiRenderID);
	glBufferData(GL_ARRAY_BUFFER, m_vecVertex.size(), m_vecVertex.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(unsigned int uiVertexSize)
{
	glCreateBuffers(1, &m_uiRenderID);
	glBindBuffer(GL_ARRAY_BUFFER, m_uiRenderID);
	glBufferData(GL_ARRAY_BUFFER, uiVertexSize, nullptr, GL_DYNAMIC_DRAW);
}

void OpenGLVertexBuffer::SetData(const void* pData, uint32_t uiVertexSize)
{
	SAND_TABLE_ASSERT(pData, "SetData is Null in OpenGLVertexBuffer");
	glBindBuffer(GL_ARRAY_BUFFER, m_uiRenderID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, uiVertexSize, pData);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	glDeleteBuffers(1, &m_uiRenderID);
}

void OpenGLVertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_uiRenderID);
}

void OpenGLVertexBuffer::UnBind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

SAND_TABLE_NAMESPACE_END
