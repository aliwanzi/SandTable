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

	void SetObjectContainer(Ref<ObjectContainer> spObjectContainer);
	const Ref<ObjectContainer>& GetObjectContainer()const;

	void SetObjectLights(Ref<ObjectContainer> spObjectLights);
	const Ref<ObjectContainer>& GetObjectLights()const;

	void AddMaterial(const Ref<Material>& spMaterial);
	void ClearMaterial();
	MapMaterial& GetMaterials();

	void SetBackGroundColor(const glm::dvec3& background);
	const glm::dvec3& GetBackGroundColor()const;

	uint32_t GetRenderImage()const;

	bool& GetAccumulate();
	int& GetFrameIndex();
	void ResetFrameIndex();
	void SaveImage();
private:
	void PreRender(Ref<RayTracingCamera>& spCamera);
	void Render(Ref<RayTracingCamera>& spCamera);
	void PostRender(Ref<RayTracingCamera>& spCamera);

	glm::dvec3 TraceRay(const Ray& ray, const Ref<Hittable>& spHittable, int depth);

private:
	MapMaterial m_mapMaterial;
	Ref<ObjectContainer> m_spObjectContainer;
	Ref<ObjectContainer> m_spObjectLights;

	bool m_bAccumulate;
	bool m_bSaveImage;
	int m_iFrameIndex;

	Ref<Image> m_spImage;
	Ref<DataBuffer> m_spAccumulateBuffer;
	std::vector<uint32_t> m_vecImageVerticalIter;
	std::vector<uint32_t> m_vecImageHorizontalInter;

	glm::dvec3 m_vec3BackGroundColor;
};

SAND_TABLE_NAMESPACE_END
