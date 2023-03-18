#pragma once
#include "SandTable/Render/Image/Image.h"
#include "SandTable/RayTracing/Ray.h"
#include "SandTable/RayTracing/RayTracingCamera.h"

SAND_TABLE_NAMESPACE_BEGIN

class RayTracingRenderImage
{
public:
	static void Init();
	static void OnResize(unsigned int uiWidth, unsigned int uiHeight);
	static void BeginScene(const Ref<RayTracingCamera>& spCamera);
	static void EndScene();
	static uint32_t GetImage();
private:
	static glm::vec4 TraceRay(const Ref<Ray>& spRay);

private:
	static Ref<Image> m_spImage;
};

SAND_TABLE_NAMESPACE_END