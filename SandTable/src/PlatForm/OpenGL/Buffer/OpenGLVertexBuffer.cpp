#include "pch.h"
#include "OpenGLVertexBuffer.h"
#include "GL/gl3w.h"

namespace SandTable
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(const std::vector<float>& vecVertics,
		const std::shared_ptr<VertexBufferLayout>& spVertexBufferLayout) :
		VertexBuffer(vecVertics,spVertexBufferLayout)
	{
		glGenBuffers(1, &m_uiRenderID);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiRenderID);
		glBufferData(GL_ARRAY_BUFFER, m_vecVertex.size() * sizeof(float), &m_vecVertex[0], GL_STATIC_DRAW);
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

}
