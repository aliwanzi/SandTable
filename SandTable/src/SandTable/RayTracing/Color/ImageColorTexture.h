#pragma once
#include "ColorTexture.h"
#include "SandTable/Core/DataBuffer.h"
SAND_TABLE_NAMESPACE_BEGIN

class ImageColorTexture :public ColorTexture
{
public:
	ImageColorTexture(const std::string& sPath);
	virtual ~ImageColorTexture() = default;
	// Í¨¹ý ColorTexture ¼Ì³Ð
	virtual glm::dvec3 GetColor(const glm::dvec3& SamplePoint, const glm::dvec2& UV) const override;
private:
	Ref<DataBuffer> m_spImageDataBuffer;
	int m_iWidth;
	int m_iHeight;
	int m_iChannel;

	unsigned char* data;
	int width, height;
	int bytes_per_scanline;

};

SAND_TABLE_NAMESPACE_END

