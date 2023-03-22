#pragma once
#include "RayTracingCamera.h"
#include "SandTable/Core/TimeStep.h"
#include "SandTable/Render/Image/Image.h"

#include "SandTable/RayTracing/Material/Material.h"
#include "SandTable/RayTracing/Object/ObjectContainer.h"

SAND_TABLE_NAMESPACE_BEGIN

class RayTracingScene
{
public:
	RayTracingScene();
	~RayTracingScene() = default;
	void OnUpdate(const TimeStep& timeStep, Ref<RayTracingCamera>& spCamera);
	void OnViewPortResize(unsigned int uiWidth, unsigned int uiHeight);

	void SetObjectContainer(std::shared_ptr<ObjectContainer> spObjectContainer);
	const std::shared_ptr<ObjectContainer>& GetObjectContainer()const;

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

	glm::dvec3 TraceRay(const Ray& ray, const std::shared_ptr<Hittable>& spHittable, int depth);

private:
	MapMaterial m_mapMaterial;
	Ref<ObjectContainer> m_spObjectContainer;

	bool m_bAccumulate;
	int m_iFrameIndex;

	Ref<Image> m_spImage;
	Ref<DataBuffer> m_spAccumulateBuffer;
	std::vector<uint32_t> m_vecImageVerticalIter;
	std::vector<uint32_t> m_vecImageHorizontalInter;
};

SAND_TABLE_NAMESPACE_END
