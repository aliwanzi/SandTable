#include "pch.h"
#include "OpenGLVertexArray.h"
#include "GL/gl3w.h"

SAND_TABLE_NAMESPACE_BEGIN

GLenum GetDataType(VertexDataType dataType)
{
	switch (dataType)
	{
	case VertexDataType::Float:		return GL_FLOAT;
	case VertexDataType::Float2:		return GL_FLOAT;
	case VertexDataType::Float3:		return GL_FLOAT;
	case VertexDataType::Float4:		return GL_FLOAT;
	case VertexDataType::Mat3:		return GL_FLOAT;
	case VertexDataType::Mat4:		return GL_FLOAT;
	case VertexDataType::Int:			return GL_INT;
	case VertexDataType::Int2:		return GL_INT;
	case VertexDataType::Int3:		return GL_INT;
	case VertexDataType::Int4:		return GL_INT;
	case VertexDataType::Bool:		return GL_BOOL;
	default:
	{
		SAND_TABLE_ASSERT(false, "Unknown DataType");
		return 0;
	}
	}
}

OpenGLVertexArray::OpenGLVertexArray()
{
	glGenVertexArrays(1, &m_uiRenderID);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
	glDeleteVertexArrays(1, &m_uiRenderID);
}

void OpenGLVertexArray::Bind() const
{
	glBindVertexArray(m_uiRenderID);
}

void OpenGLVertexArray::UnBind() const
{
	glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const Ref<Buffer>& spBuffer)
{
	auto spVertexBuffer = std::dynamic_pointer_cast<VertexBuffer>(spBuffer);
	SAND_TABLE_ASSERT(spVertexBuffer, "Vertex Buffer in OpenGLVertexArray is null");

	const auto& spVertexBufferLayout = spVertexBuffer->GetVertexBufferLayout();
	SAND_TABLE_ASSERT(spVertexBufferLayout->GetVertexBufferElements().size(), "Vertex Buffer has no layout");

	Bind();
	spVertexBuffer->Bind();

	GLuint uiIndex(0);
	for (const auto& element : spVertexBufferLayout->GetVertexBufferElements())
	{
		glEnableVertexAttribArray(uiIndex);
		glVertexAttribPointer(
			uiIndex,
			element.GetDataCount(),
			GetDataType(element.DataType),
			element.Normalized ? GL_TRUE : GL_FALSE,
			spVertexBufferLayout->GetStride(),
			(const void*)(element.Offset));
		uiIndex++;
	}

	m_vecVertexBuffer.push_back(spVertexBuffer);
}

const std::vector<Ref<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffer() const
{
	return m_vecVertexBuffer;
}

void OpenGLVertexArray::SetIndexBuffer(const Ref<Buffer>& spBuffer)
{
	m_spIndexBuffer = std::dynamic_pointer_cast<IndexBuffer>(spBuffer);
	SAND_TABLE_ASSERT(m_spIndexBuffer, "Vertex Buffer in OpenGLVertexArray is null");

	Bind();
	m_spIndexBuffer->Bind();
}

const Ref<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const
{
	return m_spIndexBuffer;
}

SAND_TABLE_NAMESPACE_END