#include "pch.h"
#include "OpenGLTexture2D.h"
#include "GL/gl3w.h"
#include "stb_image.h"

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

}

OpenGLTexture2D::OpenGLTexture2D(const std::string& sPath):
	Texture2D(sPath)
{
	stbi_set_flip_vertically_on_load(1);
	stbi_uc* pImageData = stbi_load(m_sPath.c_str(), &m_iWidth, &m_iHeight, &m_iChannel, 0);
	SAND_TABLE_ASSERT(pImageData, "Image Load in OpenGLTexture2D Failed");

	GLenum uiInternalFormat(0), uiDataFormat(0);
	GetFormat(m_iChannel, uiInternalFormat, uiDataFormat);

	glGenTextures(1, &m_uiRenderID);
	glBindTexture(GL_TEXTURE_2D, m_uiRenderID);
	glTexImage2D(GL_TEXTURE_2D, 0, uiInternalFormat, m_iWidth, m_iHeight, 0, 
		uiDataFormat, GL_UNSIGNED_BYTE, pImageData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(pImageData);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
	glDeleteTextures(1, &m_uiRenderID);
}

void OpenGLTexture2D::Bind(unsigned int uiPos) const
{
	glBindTextureUnit(uiPos, m_uiRenderID);
}


SAND_TABLE_NAMESPACE_END


