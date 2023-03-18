#pragma once
#include "RayTracingCamera.h"
#include "SandTable/Core/TimeStep.h"
#include "SandTable/Scene/Primitive/SpherePrimitive.h"

SAND_TABLE_NAMESPACE_BEGIN

class RayTracingScene
{
public:
	void OnUpdate(const TimeStep& timeStep, Ref<RayTracingCamera>& spCamera);
	void OnViewPortResize(unsigned int uiWidth, unsigned int uiHeight);

	void AddSpherePrimive(const Ref<SpherePrimitive>& spSpherePrimive);
	MapSphere& GetSpherePrimives();

	uint32_t GetRenderImage()const;
private:

	MapSphere m_mapSpherePrimitve;
};

SAND_TABLE_NAMESPACE_END
