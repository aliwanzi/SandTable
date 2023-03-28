#include "pch.h"
#include "Image.h"
#include <stb_image_write.h>

SAND_TABLE_NAMESPACE_BEGIN
Image::Image(InternalFormat eInternalFormat, DataFormat eDataFormat):
	m_spDataBuffer(CreateRef<DataBuffer>(0, eDataFormat == DataFormat::RGBA ? 4 : 3))
{
	m_spTexture = std::dynamic_pointer_cast<Texture2D>
		(Texture2D::Create(0, 0, eInternalFormat, eDataFormat));
}

Image::Image(uint32_t uiWidth, uint32_t uiHeight, InternalFormat eInternalFormat, DataFormat eDataFormat) :
	m_spDataBuffer(CreateRef<DataBuffer>(uiWidth* uiHeight, eDataFormat == DataFormat::RGBA ? 4 : 3))
{
	m_spTexture = std::dynamic_pointer_cast<Texture2D>
		(Texture2D::Create(uiWidth, uiHeight, eInternalFormat, eDataFormat));
}

uint32_t* Image::GetImageData()
{
	return m_spDataBuffer->As<uint32_t>();
}

uint32_t Image::GetWidth()const
{
	return m_spTexture->GetWidth();
}

uint32_t Image::GetHeight()const
{
	return m_spTexture->GetHeight();
}

uint32_t Image::GetImage()const
{
	return m_spTexture->GetRenderID();
}

void Image::UpdateImage()
{
	m_spTexture->SetData(m_spDataBuffer->As<uint32_t>(), m_spDataBuffer->GetDataBufferSize());
}

void Image::Resize(uint32_t uiWidth, uint32_t uiHeight)
{
	m_spDataBuffer->Resize(uiWidth * uiHeight);
	m_spTexture->Resize(uiWidth, uiHeight);
}

uint32_t Image::ConvertToRGBA(const glm::vec4& color)
{
	auto clampColor = glm::sqrt(color); //GammaÐ£Õý
	clampColor = glm::clamp(color, clampColor, glm::vec4(1.f));
	uint8_t r = static_cast<uint8_t>(clampColor.r * 255.0f);
	uint8_t g = static_cast<uint8_t>(clampColor.g * 255.0f);
	uint8_t b = static_cast<uint8_t>(clampColor.b * 255.0f);
	uint8_t a = static_cast<uint8_t>(clampColor.a * 255.0f);
	uint32_t result = a << 24 | b << 16 | g << 8 | r;
	return result;
}

void Image::SaveImagePNG(const std::string& sPath, void* data, uint32_t width, uint32_t height, uint32_t channel)
{
	stbi_flip_vertically_on_write(1);
	stbi_write_png(sPath.c_str(), width, height, channel, data, 0);
}

SAND_TABLE_NAMESPACE_END