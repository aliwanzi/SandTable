#include "pch.h"
#include "Image.h"
#include "SandTable/Math/Random.h"

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

SAND_TABLE_NAMESPACE_END