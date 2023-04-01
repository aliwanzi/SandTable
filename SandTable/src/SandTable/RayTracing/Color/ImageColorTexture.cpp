#include "pch.h"
#include "ImageColorTexture.h"
#include "stb_image.h"

SAND_TABLE_NAMESPACE_BEGIN
ImageColorTexture::ImageColorTexture(const std::string& filename)
{
	stbi_set_flip_vertically_on_load(1);

	stbi_uc* pImageData = stbi_load(filename.c_str(), &m_iWidth, &m_iHeight, &m_iChannel, 0);
	SAND_TABLE_ASSERT(pImageData, "Image Load in ImageColorTexture Failed");

	m_spImageDataBuffer = CreateRef<DataBuffer>(m_iWidth * m_iHeight, m_iChannel);
	m_spImageDataBuffer->SetDataBuffer(pImageData);

	stbi_image_free(pImageData);
}



glm::dvec3 ImageColorTexture::GetColor(const glm::dvec3& SamplePoint, const glm::dvec2& UV) const
{
	if (m_spImageDataBuffer == nullptr)
		return glm::dvec3(0, 1, 1);

	uint32_t u = glm::clamp(0, static_cast<int>(UV.x * m_iWidth), m_iWidth - 1);
	uint32_t v = glm::clamp(0, static_cast<int>(UV.y * m_iHeight), m_iHeight - 1);
	auto data = m_spImageDataBuffer->As<stbi_uc>();

	SAND_TABLE_ASSERT(u >= 0 && u < m_iWidth, "u is out of width,{0}", u);
	SAND_TABLE_ASSERT(v >= 0 && v < m_iHeight, "v is out of height,{0}", v);
	SAND_TABLE_ASSERT(data, "data is null");

	auto pixel = v * m_iWidth + u;
	return glm::dvec3(data[pixel * m_iChannel] / 255.0, data[pixel * m_iChannel + 1] / 255.0, data[pixel * m_iChannel + 2] / 255.0);
}

SAND_TABLE_NAMESPACE_END