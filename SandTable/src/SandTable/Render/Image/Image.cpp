#include "pch.h"
#include "Image.h"

SAND_TABLE_NAMESPACE_BEGIN

Image::Image(uint32_t uiWidth, uint32_t uiHeight, InternalFormat eInternalFormat, DataFormat eDataFormat) :
	m_spDataBuffer(CreateRef<DataBuffer>(uiWidth* uiHeight * 4))
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
	m_spTexture->SetData(m_spDataBuffer->As<uint32_t>(), m_spDataBuffer->GetDataBufferSize() / 4);
	return m_spTexture->GetRenderID();
}

SAND_TABLE_NAMESPACE_END