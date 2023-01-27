#include "pch.h"
#include "OpenGLFrameBuffer.h"
#include "GL/gl3w.h"

SAND_TABLE_NAMESPACE_BEGIN
namespace
{
	void CreateTextures(GLuint* uiTextureID, unsigned int uiCount)
	{
		glGenTextures(uiCount, uiTextureID);
	}

	void BindTexture(bool bMultisample, GLuint uiTextureID)
	{
		if (bMultisample)
		{
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, uiTextureID);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, uiTextureID);
		}
	}

	void AttachColorTexture(GLuint uiTextureID, int iSamples, GLenum internalFormat, GLenum format,
		unsigned int uiWidth, unsigned int uiHeight, GLuint uiIndex)
	{
		if (iSamples > 1)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, iSamples, internalFormat, uiWidth, uiHeight, GL_TRUE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + uiIndex, GL_TEXTURE_2D_MULTISAMPLE, uiTextureID, 0);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, uiWidth, uiHeight, 0, format, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + uiIndex, GL_TEXTURE_2D, uiTextureID, 0);
		}
	}

	void AttachRenderBuffer(GLuint uiRenderBuffer, int iSamples, GLenum format, GLenum attachmentType,
		unsigned int uiWidth, unsigned int uiHeight)
	{
		if (iSamples > 1)
		{
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, iSamples, format, uiWidth, uiHeight);
		}
		else
		{
			glRenderbufferStorage(GL_RENDERBUFFER, format, uiWidth, uiHeight);
		}
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentType, GL_RENDERBUFFER, uiRenderBuffer);
	}

	bool IsDepthFormat(FramebufferTextureFormat format)
	{
		switch (format)
		{
		case SandTable::FramebufferTextureFormat::DEPTH24_STENCIL8:
			return true;
		}
		return false;
	}

	GLenum TextureFormatToGL(FramebufferTextureFormat format)
	{
		switch (format)
		{
		case SandTable::FramebufferTextureFormat::RED_INTERGER:
			return GL_RED_INTEGER;
		case SandTable::FramebufferTextureFormat::RGBA8:
			return GL_RGBA8;
		}
		return 0;
	}
}

OpenGLFrameBuffer::OpenGLFrameBuffer(const Ref<FrameBufferSpecification>& spFrameBufferSpecification):
	FrameBuffer(spFrameBufferSpecification)
{
	glCreateFramebuffers(1, &m_uiRenderID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_uiRenderID);

	for (auto attachment: spFrameBufferSpecification->m_spAttachment->Attachments)
	{
		if (!IsDepthFormat(attachment.TextureFormat))
		{
			m_vecColorAttachmentSpecifications.emplace_back(attachment);
		}
		else
		{
			m_spDepthAttachmentSpecification = CreateRef<FrameBufferTextureSpecification>(attachment);
		}
	}

	if (m_vecColorAttachmentSpecifications.size() > 0)
	{
		std::vector<GLenum> vecColorBuffers;
		m_vecColorAttachment.resize(m_vecColorAttachmentSpecifications.size());
		CreateTextures(m_vecColorAttachment.data(), m_vecColorAttachment.size());
		for (int i = 0; i < m_vecColorAttachment.size(); i++)
		{
			BindTexture(m_spFrameBufferSpecification->Samples > 1, m_vecColorAttachment[i]);
			switch (m_vecColorAttachmentSpecifications[i].TextureFormat)
			{
				case SandTable::FramebufferTextureFormat::RGBA8:
				{
					AttachColorTexture(m_vecColorAttachment[i], m_spFrameBufferSpecification->Samples, GL_RGBA8, GL_RGB,
						m_spFrameBufferSpecification->Width, m_spFrameBufferSpecification->Height, i);
					break;
				}
				case SandTable::FramebufferTextureFormat::RED_INTERGER:
				{
					AttachColorTexture(m_vecColorAttachment[i], m_spFrameBufferSpecification->Samples, GL_R32I, GL_RED_INTEGER,
						m_spFrameBufferSpecification->Width, m_spFrameBufferSpecification->Height, i);
					break;
				}
			}
			vecColorBuffers.emplace_back(GL_COLOR_ATTACHMENT0 + i);
		}
		glDrawBuffers(m_vecColorAttachment.size(), vecColorBuffers.data());
	}
	else
	{
		// Only depth-pass
		glDrawBuffer(GL_NONE);
	}

	if (m_spDepthAttachmentSpecification != nullptr)
	{
		glGenRenderbuffers(1, &m_uiDepthAttachment);
		glBindRenderbuffer(GL_RENDERBUFFER, m_uiDepthAttachment);
		AttachRenderBuffer(m_uiDepthAttachment, m_spFrameBufferSpecification->Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT,
			m_spFrameBufferSpecification->Width, m_spFrameBufferSpecification->Height);
	}

	SAND_TABLE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OpenGLFrameBuffer::~OpenGLFrameBuffer()
{
	glDeleteFramebuffers(1, &m_uiRenderID);
	glDeleteTextures(m_vecColorAttachment.size(), m_vecColorAttachment.data());
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

int OpenGLFrameBuffer::ReadPixel(unsigned int uiIndex, unsigned int uiX, unsigned int uiY) const
{
	glReadBuffer(GL_COLOR_ATTACHMENT0 + uiIndex);
	int uiPixelData;
	glReadPixels(uiX, uiY, 1, 1, GL_RED_INTEGER, GL_INT, &uiPixelData);
	return uiPixelData;
}

void OpenGLFrameBuffer::ClearColorAttachment(unsigned int uiIndex, int iValue) const
{
	auto& spe = m_vecColorAttachmentSpecifications[uiIndex];
	glClearTexImage(m_vecColorAttachment[uiIndex], 0,
		TextureFormatToGL(m_vecColorAttachmentSpecifications[uiIndex].TextureFormat),
		GL_INT, &iValue);
}

void OpenGLFrameBuffer::Resize(unsigned int uiWidth, unsigned int uiHeight)
{
	if ((uiWidth == m_spFrameBufferSpecification->Width && uiHeight == m_spFrameBufferSpecification->Height)
		|| (uiWidth <= 0 || uiHeight <= 0))
	{
		//LOG_DEV_ERROR("Attachment to resize framebuffer to {0},{1}", uiWidth, uiHeight);
		return;
	}

	m_spFrameBufferSpecification->Width = uiWidth;
	m_spFrameBufferSpecification->Height = uiHeight;

	glBindFramebuffer(GL_FRAMEBUFFER, m_uiRenderID);

	for (int i = 0; i < m_vecColorAttachment.size(); i++)
	{
		BindTexture(m_spFrameBufferSpecification->Samples > 1, m_vecColorAttachment[i]);
		switch (m_vecColorAttachmentSpecifications[i].TextureFormat)
		{
			case SandTable::FramebufferTextureFormat::RGBA8:
			{
				AttachColorTexture(m_vecColorAttachment[i], m_spFrameBufferSpecification->Samples, GL_RGBA8, GL_RGB,
					m_spFrameBufferSpecification->Width, m_spFrameBufferSpecification->Height, i);
				break;
			}
			case SandTable::FramebufferTextureFormat::RED_INTERGER:
			{
				AttachColorTexture(m_vecColorAttachment[i], m_spFrameBufferSpecification->Samples, GL_R32I, GL_RED_INTEGER,
					m_spFrameBufferSpecification->Width, m_spFrameBufferSpecification->Height, i);
				break;
			}
		}
	}

	if (m_spDepthAttachmentSpecification != nullptr)
	{
		glGenRenderbuffers(1, &m_uiDepthAttachment);
		glBindRenderbuffer(GL_RENDERBUFFER, m_uiDepthAttachment);
		if (m_spFrameBufferSpecification->Samples > 1)
		{
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_spFrameBufferSpecification->Samples, GL_DEPTH24_STENCIL8,
				m_spFrameBufferSpecification->Width, m_spFrameBufferSpecification->Height);
		}
		else
		{
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
				m_spFrameBufferSpecification->Width, m_spFrameBufferSpecification->Height);
		}
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_uiDepthAttachment);
	}

	SAND_TABLE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

SAND_TABLE_NAMESPACE_END


