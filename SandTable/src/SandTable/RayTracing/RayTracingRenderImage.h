#pragma once
#include "SandTable/Render/Image/Image.h"
#include "SandTable/RayTracing/Ray.h"
#include "SandTable/RayTracing/RayTracingCamera.h"
#include "SandTable/RayTracing/RayTracingScene.h"

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
		Ref<DataBuffer> spAccumulateBuffer;
		glm::mat4 matView;
		glm::mat4 matProjection;
		std::vector<uint32_t> vecImageVerticalIter;
		std::vector<uint32_t> vecImageHorizontalInter;
	};

	static void Init();
	static void OnResize(unsigned int uiWidth, unsigned int uiHeight);
	static void BeginScene(const Ref<RayTracingCamera>& spCamera);
	static void EndScene();
	static void RenderPrimitve(Ref<RayTracingScene>& spRayTracingScene);
	static uint32_t GetImage();

private:
	static glm::vec4 PerPixel(const glm::vec3& rayOrigin, uint32_t uiX, uint32_t uiY, Ref<RayTracingScene>& spScene);
	static void TraceRay(const Ray& ray, const MapSphere& mapSpherePrimitive, HitPayLoad& hitPayLoad);
private:
	static Ref<RenderStroge> m_spRenderStroge;
};

SAND_TABLE_NAMESPACE_END