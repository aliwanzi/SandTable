#pragma once

SAND_TABLE_NAMESPACE_BEGIN

class Texture
{
public:
	virtual ~Texture() = default;
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
	virtual void SetData(void* pData, unsigned int uiSize) = 0;
	virtual void Bind(unsigned int uiPos = 0) const = 0;
	virtual void UnBind()const = 0;
	virtual unsigned int GetRenderID() const = 0;
};

SAND_TABLE_NAMESPACE_END

