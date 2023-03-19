#pragma once
#include "RayTracingCamera.h"
#include "SandTable/Core/TimeStep.h"
#include "SandTable/Scene/Primitive/SpherePrimitive.h"
#include "SandTable/RayTracing/Material.h"

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
	void CheckDirty(Ref<RayTracingCamera>& spCamera);
	void ResetDirty(Ref<RayTracingCamera>& spCamera);
private:
	MapMaterial m_mapMaterial;
	MapSphere m_mapSpherePrimitve;
	bool m_bAccumulate;
	int m_iFrameIndex;
};

SAND_TABLE_NAMESPACE_END
