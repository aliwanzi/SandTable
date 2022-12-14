#pragma once
#include "SandTable/Render/Texture/Texture2D.h"
SAND_TABLE_NAMESPACE_BEGIN

class OpenGLTexture2D :public Texture2D
{
public:
	OpenGLTexture2D(int iWidth,int iHeight);
	OpenGLTexture2D(const std::string& sPath);
	~OpenGLTexture2D();
	virtual void Bind(unsigned int uiPos = 0) const override;
	virtual void SetData(void* pData, unsigned int uiSize) override;
	virtual void UnBind()const override;
private:
	unsigned int m_uiInternalFormat;
	unsigned int m_uiDataFormat;
};

SAND_TABLE_NAMESPACE_END

