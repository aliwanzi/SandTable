#pragma once
#include "SandTable/Scene/Primitive/SpherePrimitive.h"
#include "RayTracingCamera.h"
#include "SandTable/Core/TimeStep.h"

SAND_TABLE_NAMESPACE_BEGIN

class RayTracingScene
{
public:
	void OnUpdate(const TimeStep& timeStep, const Ref<RayTracingCamera>& spCamera);
	void OnViewPortResize(unsigned int uiWidth, unsigned int uiHeight);
	uint32_t GetRenderImage()const;
private:

	std::vector<SpherePrimitive> m_vecSpherePrimitve;
};

SAND_TABLE_NAMESPACE_END
