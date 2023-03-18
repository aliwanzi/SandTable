#pragma once
#include "SandTable/Render/Image/Image.h"
#include "SandTable/RayTracing/Ray.h"
#include "SandTable/RayTracing/RayTracingCamera.h"

SAND_TABLE_NAMESPACE_BEGIN
class SpherePrimitive;
class RayTracingMaterial;
class RayTracingRenderImage
{
public:
	struct RenderStroge
	{
		Ref<Ray> spRay;
		Ref<Image> spImage;
		glm::mat4 matView;
		glm::mat4 matProjection;
	};

	static void Init();
	static void OnResize(unsigned int uiWidth, unsigned int uiHeight);
	static void BeginScene(const Ref<RayTracingCamera>& spCamera);
	static void EndScene();
	static void RenderPrimitve(const MapSphere& mapSpherePrimitive);
	static uint32_t GetImage();

private:
	static glm::vec4 PerPixel(Ref<Ray> spRay, const MapSphere& mapSpherePrimitive);
	static Ref<HitPayLoad> TraceRay(const Ref<Ray>& spRay, const MapSphere& mapSpherePrimitive);
private:
	static Ref<RenderStroge> m_spRenderStroge;
};

SAND_TABLE_NAMESPACE_END