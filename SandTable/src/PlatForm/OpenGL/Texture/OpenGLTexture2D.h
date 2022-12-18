#pragma once
#include "SandTable/Render/Texture/Texture2D.h"
SAND_TABLE_NAMESPACE_BEGIN

class OpenGLTexture2D :public Texture2D
{
public:
	OpenGLTexture2D(const std::string& sPath);
	~OpenGLTexture2D();
	virtual void Bind(unsigned int uiPos = 0) const override;
};

SAND_TABLE_NAMESPACE_END

