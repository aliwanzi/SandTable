#include "pch.h"
#include "OpenGLTexture2D.h"
#include "GL/gl3w.h"
#include "stb_image.h"
#include "SandTable/Statics/Instrumentor.h"

SAND_TABLE_NAMESPACE_BEGIN
namespace
{
	void GetFormat(int iChannels, GLenum& uiInternalFormat, GLenum& uiDataFormat)
	{
		switch (iChannels)
		{
		case 1:
		{
			uiInternalFormat = GL_RED;
			uiDataFormat = GL_RED;
			break;
		}
		case 3:
		{
			uiInternalFormat = GL_RGB8;
			uiDataFormat =  GL_RGB;
			break;
		}
		case 4:
		{
			uiInternalFormat = GL_RGBA8;
			uiDataFormat = GL_RGBA;
			break;
		}
		default:
		{
			LOG_DEV_ERROR("Unkown Image Data Channel");
			break;
		}
		}
	}

	GLenum InternalFormatToOpenGL(InternalFormat eInternalFormat)
	{
		switch (eInternalFormat)
		{
			case InternalFormat::NONE:
				return 0;
			case InternalFormat::RED:
				return GL_RED;
			case InternalFormat::RGB:
				return GL_RGB;
			case InternalFormat::RGBA:
				return GL_RGBA;
			case InternalFormat::RGBA8:
				return GL_RGBA8;
			case InternalFormat::RGBA32F:
				return GL_RGBA32F;
			default:
			{
				LOG_DEV_ERROR("Unkown Internal Format");
				return 0;
			}
		}
	}

	GLenum DataFormatToOpenGL(DataFormat eDataFormat)
	{
		switch (eDataFormat)
		{
			case DataFormat::NONE:
				return 0;
			case DataFormat::RED:
				return GL_RED;
			case DataFormat::RGB:
				return GL_RGB;
			case DataFormat::RGBA:
				return GL_RGBA;
			default:
			{
				LOG_DEV_ERROR("Unkown Data Format");
				return 0;
			}
		}
	}
}

OpenGLTexture2D::OpenGLTexture2D(int iWidth, int iHeight, InternalFormat eInternalFormat, DataFormat eDataFormat) :
	m_uiInternalFormat(InternalFormatToOpenGL(eInternalFormat)), m_uiDataFormat(DataFormatToOpenGL(eDataFormat)), Texture2D(iWidth, iHeight)
{
	SAND_TABLE_PROFILE_FUNCTION();
	glGenTextures(1, &m_uiRenderID);
	glBindTexture(GL_TEXTURE_2D, m_uiRenderID);
	glTexImage2D(GL_TEXTURE_2D, 0, m_uiInternalFormat, m_iWidth, m_iHeight, 0,
		m_uiDataFormat, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& sPath):
	Texture2D(sPath)
{
	SAND_TABLE_PROFILE_FUNCTION();
	stbi_set_flip_vertically_on_load(1);
	stbi_uc* pImageData = nullptr;
	{
		SAND_TABLE_PROFILE_SCOPE("Load Data in OpenGLTexture2D.");
		pImageData = stbi_load(m_sPath.c_str(), &m_iWidth, &m_iHeight, &m_iChannel, 0);
	}

	SAND_TABLE_ASSERT(pImageData, "Image Load in OpenGLTexture2D Failed");

	GetFormat(m_iChannel, m_uiInternalFormat, m_uiDataFormat);

	glGenTextures(1, &m_uiRenderID);
	glBindTexture(GL_TEXTURE_2D, m_uiRenderID);
	glTexImage2D(GL_TEXTURE_2D, 0, m_uiInternalFormat, m_iWidth, m_iHeight, 0,
		m_uiDataFormat, GL_UNSIGNED_BYTE, pImageData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(pImageData);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
	glDeleteTextures(1, &m_uiRenderID);
}

void OpenGLTexture2D::Bind(unsigned int uiPos) const
{
	glActiveTexture(GL_TEXTURE0 + uiPos);
	glBindTexture(GL_TEXTURE_2D, m_uiRenderID);
}

void OpenGLTexture2D::SetData(const void* pData, unsigned int uiSize)
{
	SAND_TABLE_PROFILE_FUNCTION();
	int iChannel = m_uiDataFormat == GL_RGBA ? 4 : 3;
	SAND_TABLE_ASSERT(uiSize = m_iWidth * m_iHeight * iChannel, "Data must be entire in OpenGLTexture2D");
	SAND_TABLE_ASSERT(pData, "Data is null in OpenGLTexture2D");
	glTextureSubImage2D(m_uiRenderID, 0, 0, 0, m_iWidth, m_iHeight, m_uiDataFormat, GL_UNSIGNED_BYTE, pData);
}

void OpenGLTexture2D::UnBind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLTexture2D::Resize(unsigned int uiWidth, unsigned int uiHeight)
{
	if (uiWidth != m_iWidth || uiHeight != m_iHeight)
	{
		Texture2D::Resize(uiWidth, uiHeight);
		glBindTexture(GL_TEXTURE_2D, m_uiRenderID);
		glTexImage2D(GL_TEXTURE_2D, 0, m_uiInternalFormat, m_iWidth, m_iHeight, 0,
			m_uiDataFormat, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}


SAND_TABLE_NAMESPACE_END


