#pragma once
#include "SandTable/Render/Texture/Texture2D.h"
#include "SandTable/Core/DataBuffer.h"

SAND_TABLE_NAMESPACE_BEGIN

class Image
{
public:
	Image(InternalFormat eInternalFormat = InternalFormat::RGBA32F, DataFormat eDataFormat = DataFormat::RGBA);
	Image(uint32_t uiWidth, uint32_t uiHeight,
		InternalFormat eInternalFormat = InternalFormat::RGBA32F, DataFormat eDataFormat = DataFormat::RGBA);

	uint32_t GetWidth()const;
	uint32_t GetHeight()const;

	void UpdateImage();
	uint32_t GetImage()const;
	uint32_t* GetImageData();

	void Resize(uint32_t uiWidth, uint32_t uiHeight);

	static uint32_t ConvertToRGBA(const glm::vec4& color);
private:
	Ref<Texture2D> m_spTexture;
	Ref<DataBuffer> m_spDataBuffer;
};

SAND_TABLE_NAMESPACE_END


