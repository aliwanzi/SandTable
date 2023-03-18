#include "pch.h"
#include "RayTracingScene.h"
#include "RayTracingRenderImage.h"

SAND_TABLE_NAMESPACE_BEGIN

void RayTracingScene::OnUpdate(const TimeStep& timeStep, Ref<RayTracingCamera>& spCamera)
{
	RayTracingRenderImage::BeginScene(spCamera);
	RayTracingRenderImage::RenderPrimitve(m_mapSpherePrimitve);
	RayTracingRenderImage::EndScene();
}

void RayTracingScene::OnViewPortResize(unsigned int uiWidth, unsigned int uiHeight)
{
	RayTracingRenderImage::OnResize(uiWidth, uiHeight);
}

void RayTracingScene::AddSpherePrimive(const Ref<SpherePrimitive>& spSpherePrimive)
{
	m_mapSpherePrimitve[spSpherePrimive->GetEntityID()] = spSpherePrimive;
}

MapSphere& RayTracingScene::GetSpherePrimives()
{
	return m_mapSpherePrimitve;
}

uint32_t RayTracingScene::GetRenderImage() const
{
	return RayTracingRenderImage::GetImage();
}

SAND_TABLE_NAMESPACE_END
