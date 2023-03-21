#pragma once
#include "RayTracingCamera.h"
#include "SandTable/Core/TimeStep.h"
#include "SandTable/Scene/Primitive/SpherePrimitive.h"
#include "SandTable/RayTracing/Material.h"
#include "SandTable/Render/Image/Image.h"

SAND_TABLE_NAMESPACE_BEGIN

class RayTracingScene :public std::enable_shared_from_this<RayTracingScene>
{
public:
	RayTracingScene();
	~RayTracingScene() = default;
	void OnUpdate(const TimeStep& timeStep, Ref<RayTracingCamera>& spCamera);
	void OnViewPortResize(unsigned int uiWidth, unsigned int uiHeight);

	void AddSpherePrimive(const Ref<SpherePrimitive>& spSpherePrimive);
	MapSphere& GetSpherePrimives();

	void AddMaterial(const Ref<Material>& spMaterial);
	MapMaterial& GetMaterials();

	uint32_t GetRenderImage()const;

	bool& GetAccumulate();
	int& GetFrameIndex();
	void ResetFrameIndex();
private:
	void PreRender(Ref<RayTracingCamera>& spCamera);
	void Render(Ref<RayTracingCamera>& spCamera);
	void PostRender(Ref<RayTracingCamera>& spCamera);

	glm::vec4 PerPixel(const glm::vec3& rayOrigin, uint32_t uiX, uint32_t uiY);
	void TraceRay(const Ray& ray, HitPayLoad& hitPayLoad);

private:
	MapMaterial m_mapMaterial;
	MapSphere m_mapSpherePrimitve;

	bool m_bAccumulate;
	int m_iFrameIndex;

	glm::mat4 m_matView;
	glm::mat4 m_matProjection;

	Ref<Ray> m_spRay;
	Ref<Image> m_spImage;
	Ref<DataBuffer> m_spAccumulateBuffer;
	std::vector<uint32_t> m_vecImageVerticalIter;
	std::vector<uint32_t> m_vecImageHorizontalInter;
};

SAND_TABLE_NAMESPACE_END
