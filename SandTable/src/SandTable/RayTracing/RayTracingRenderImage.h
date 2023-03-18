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
	static void RenderPrimitve(const std::vector<Ref<SpherePrimitive>>& vecSpherePrimitive);
	static uint32_t GetImage();
private:
	static glm::vec4 TraceRay(const std::vector<Ref<SpherePrimitive>>& vecSpherePrimitive);

private:
	static Ref<RenderStroge> m_spRenderStroge;
};

SAND_TABLE_NAMESPACE_END