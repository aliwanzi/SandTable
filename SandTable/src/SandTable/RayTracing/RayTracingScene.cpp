#include "pch.h"
#include "RayTracingScene.h"
#include "RayTracingRenderImage.h"

SAND_TABLE_NAMESPACE_BEGIN

void RayTracingScene::OnUpdate(const TimeStep& timeStep, const Ref<RayTracingCamera>& spCamera)
{
	RayTracingRenderImage::BeginScene(spCamera);
}

void RayTracingScene::OnViewPortResize(unsigned int uiWidth, unsigned int uiHeight)
{
	RayTracingRenderImage::OnResize(uiWidth, uiHeight);
}

uint32_t RayTracingScene::GetRenderImage() const
{
	return RayTracingRenderImage::GetImage();
}

SAND_TABLE_NAMESPACE_END
