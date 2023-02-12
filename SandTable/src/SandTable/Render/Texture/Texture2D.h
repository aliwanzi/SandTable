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
	static Ref<Texture> Create(unsigned int uiWidth, unsigned int uiHeight);
	virtual unsigned int GetRenderID() const override;
	virtual const std::string& GetTexturePath()const;
protected:
	Texture2D(const std::string& sPath);
	Texture2D(int iWidth, int iHeight);
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
	unsigned int m_uiInternalFormat;
	unsigned int m_uiDataFormat;
};

SAND_TABLE_NAMESPACE_END