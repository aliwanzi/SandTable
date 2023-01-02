#include "pch.h"
#include "OpenGLFrameBuffer.h"
#include "GL/gl3w.h"

SAND_TABLE_NAMESPACE_BEGIN
OpenGLFrameBuffer::OpenGLFrameBuffer(const Ref<FrameBufferSpecification>& spFrameBufferSpecification):
	FrameBuffer(spFrameBufferSpecification)
{
	glCreateFramebuffers(1, &m_uiRenderID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_uiRenderID);

	glGenTextures(1, &m_uiColorAttachment);
	glBindTexture(GL_TEXTURE_2D, m_uiColorAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_spFrameBufferSpecification->Width, m_spFrameBufferSpecification->Height, 
		0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_uiColorAttachment, 0);

	//glGenTextures(1, &m_uiDepthAttachment);
	//glBindTexture(GL_TEXTURE_2D, m_uiDepthAttachment);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_spFrameBufferSpecification->Width, m_spFrameBufferSpecification->Height,
	//	0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_uiDepthAttachment, 0);

	glGenRenderbuffers(1, &m_uiDepthAttachment);
	glBindRenderbuffer(GL_RENDERBUFFER, m_uiDepthAttachment);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_spFrameBufferSpecification->Width, m_spFrameBufferSpecification->Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_uiDepthAttachment);

	SAND_TABLE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

OpenGLFrameBuffer::~OpenGLFrameBuffer()
{
	glDeleteFramebuffers(1, &m_uiRenderID);
	glDeleteTextures(1, &m_uiColorAttachment);
	glDeleteRenderbuffers(1, &m_uiDepthAttachment);
}

void OpenGLFrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_uiRenderID);
	glViewport(0, 0, m_spFrameBufferSpecification->Width, m_spFrameBufferSpecification->Height);
}

void OpenGLFrameBuffer::UnBind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFrameBuffer::Resize(unsigned int uiWidth, unsigned int uiHeight)
{
	if ((uiWidth == m_spFrameBufferSpecification->Width && uiHeight != m_spFrameBufferSpecification->Height)
		|| (uiWidth <= 0 || uiHeight <= 0))
	{
		LOG_DEV_ERROR("Attachment to resize framebuffer to {0},{1}", uiWidth, uiHeight);
		return;
	}

	m_spFrameBufferSpecification->Width = uiWidth;
	m_spFrameBufferSpecification->Height = uiHeight;

	glBindFramebuffer(GL_FRAMEBUFFER, m_uiRenderID);

	glBindTexture(GL_TEXTURE_2D, m_uiColorAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_spFrameBufferSpecification->Width, m_spFrameBufferSpecification->Height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_uiColorAttachment, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, m_uiDepthAttachment);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_spFrameBufferSpecification->Width, m_spFrameBufferSpecification->Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_uiDepthAttachment);

	SAND_TABLE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

SAND_TABLE_NAMESPACE_END


