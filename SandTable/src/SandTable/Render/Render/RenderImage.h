#pragma once
#include "SandTable/Render/Render/Render.h"
#include "SandTable/Render/Image/Image.h"

SAND_TABLE_NAMESPACE_BEGIN
class RenderImage
{
public:
	static void Init();
	static void OnWindowResize(unsigned int uiWidth, unsigned int uiHeight);
	static void OnRender();
	static uint32_t GetImage();
private:
	static uint32_t GetPerPixel(const glm::vec2& vec2Coord);

private:
	static Ref<Image> m_spImage;
};

SAND_TABLE_NAMESPACE_END