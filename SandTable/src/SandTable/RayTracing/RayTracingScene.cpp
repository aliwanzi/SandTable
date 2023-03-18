#include "pch.h"
#include "RayTracingScene.h"
#include "RayTracingRenderImage.h"

SAND_TABLE_NAMESPACE_BEGIN

void RayTracingScene::OnUpdate(const TimeStep& timeStep, const Ref<RayTracingCamera>& spCamera)
{
	RayTracingRenderImage::BeginScene(spCamera);
	RayTracingRenderImage::RenderPrimitve(m_vecSpherePrimitve);
	RayTracingRenderImage::EndScene();
}

void RayTracingScene::OnViewPortResize(unsigned int uiWidth, unsigned int uiHeight)
{
	RayTracingRenderImage::OnResize(uiWidth, uiHeight);
}

void RayTracingScene::AddSpherePrimive(const Ref<SpherePrimitive>& spSpherePrimive)
{
	m_vecSpherePrimitve.push_back(spSpherePrimive);
}

std::vector<Ref<SpherePrimitive>>& RayTracingScene::GetSpherePrimives()
{
	return m_vecSpherePrimitve;
}

uint32_t RayTracingScene::GetRenderImage() const
{
	return RayTracingRenderImage::GetImage();
}

SAND_TABLE_NAMESPACE_END
