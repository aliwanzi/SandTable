#pragma once
#include "SandTable/Scene/Primitive/SpherePrimitive.h"
#include "SandTable/RayTracing/RayTracingMaterial.h"

#include "RayTracingCamera.h"
#include "SandTable/Core/TimeStep.h"

SAND_TABLE_NAMESPACE_BEGIN

class RayTracingScene
{
public:
	void OnUpdate(const TimeStep& timeStep, const Ref<RayTracingCamera>& spCamera);
	void OnViewPortResize(unsigned int uiWidth, unsigned int uiHeight);

	void AddSpherePrimive(const Ref<SpherePrimitive>& spSpherePrimive);
	std::vector<Ref<SpherePrimitive>>& GetSpherePrimives();

	uint32_t GetRenderImage()const;
private:

	std::vector<Ref<SpherePrimitive>> m_vecSpherePrimitve;
};

SAND_TABLE_NAMESPACE_END
