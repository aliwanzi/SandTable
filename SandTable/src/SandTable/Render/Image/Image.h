#pragma once
#include "SandTable/Render/Texture/Texture2D.h"
#include "SandTable/Core/DataBuffer.h"

SAND_TABLE_NAMESPACE_BEGIN

class Image
{
public:
	Image(uint32_t uiWidth, uint32_t uiHeight, InternalFormat eInternalFormat, DataFormat eDataFormat);
	uint32_t* GetImageData();
	uint32_t GetWidth()const;
	uint32_t GetHeight()const;
	uint32_t GetImage()const;
private:
	Ref<Texture2D> m_spTexture;
	Ref<DataBuffer> m_spDataBuffer;
};

SAND_TABLE_NAMESPACE_END


