#include "pch.h"
#include "RenderImage.h"

SAND_TABLE_NAMESPACE_BEGIN

Ref<Image> RenderImage::m_spImage = nullptr;
void RenderImage::Init()
{
	m_spImage = CreateRef<Image>();
}

void RenderImage::OnWindowResize(unsigned int uiWidth, unsigned int uiHeight)
{
	if (uiWidth != m_spImage->GetWidth() || uiHeight != m_spImage->GetHeight())
	{
		m_spImage->Resize(uiWidth, uiHeight);
	}
}

void RenderImage::OnRender()
{
	auto width = m_spImage->GetWidth();
	auto height = m_spImage->GetHeight();

	auto pImageData = m_spImage->GetImageData();
	for (uint32_t y = 0; y < height; y++)
	{
		for (uint32_t x = 0; x < width; x++)
		{
			glm::vec2 vec2Coord = { static_cast<float>(x) / static_cast<float>(width),
				static_cast<float>(y) / static_cast<float>(height) };
			pImageData[x + y * width] = GetPerPixel(vec2Coord);
		}
	}
	m_spImage->UpdateImage();
}

uint32_t RenderImage::GetImage()
{
	return m_spImage->GetImage();
}

uint32_t RenderImage::GetPerPixel(const glm::vec2& vec2Coord)
{
	uint8_t r = static_cast<uint8_t>(vec2Coord.x * UCHAR_MAX);
	uint8_t g = static_cast<uint8_t>(vec2Coord.y * UCHAR_MAX);
	return 0xff000000 | (g << 8) | r;
}

SAND_TABLE_NAMESPACE_END