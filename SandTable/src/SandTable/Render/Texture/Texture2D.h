#pragma once
#include "SandTable/Render/Texture/Texture.h"

SAND_TABLE_NAMESPACE_BEGIN

class Texture2D :public Texture
{
public:
	// Í¨¹ý Texture ¼Ì³Ð
	virtual int GetWidth() const override;
	virtual int GetHeight() const override;
	static Ref<Texture> Create(const std::string& sPath);
protected:
	Texture2D(const std::string& sPath);
private:
	Texture2D() = delete;
	Texture2D(Texture2D&) = delete;
	Texture2D& operator=(const Texture2D&) = delete;

protected:
	std::string m_sPath;
	int m_iWidth;
	int m_iHeight;
	int m_iChannel;

	unsigned int m_uiRenderID;
};

SAND_TABLE_NAMESPACE_END