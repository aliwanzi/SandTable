#include "pch.h"
#include "RayTracingScene.h"
#include "RayTracingRenderImage.h"

SAND_TABLE_NAMESPACE_BEGIN

RayTracingScene::RayTracingScene():
	m_iFrameIndex(1),
	m_bAccumulate(false)
{
}

void RayTracingScene::OnUpdate(const TimeStep& timeStep, Ref<RayTracingCamera>& spCamera)
{
	CheckDirty(spCamera);
	RayTracingRenderImage::BeginScene(spCamera);
	RayTracingRenderImage::RenderPrimitve(shared_from_this());
	RayTracingRenderImage::EndScene();
	ResetDirty(spCamera);
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

void RayTracingScene::AddMaterial(const Ref<Material>& spMaterial)
{
	m_mapMaterial[spMaterial->GetMaterialID()] = spMaterial;
}

MapMaterial& RayTracingScene::GetMaterials()
{
	return m_mapMaterial;
}

uint32_t RayTracingScene::GetRenderImage() const
{
	return RayTracingRenderImage::GetImage();
}

bool& RayTracingScene::GetAccumulate()
{
	return m_bAccumulate;
}

int& RayTracingScene::GetFrameIndex()
{
	return m_iFrameIndex;
}

void RayTracingScene::ResetFrameIndex()
{
	m_iFrameIndex = 1;
}

void RayTracingScene::CheckDirty(Ref<RayTracingCamera>& spCamera)
{
	bool bDirty(spCamera->GetDirty());
	if (!bDirty)
	{
		for (auto& sphere : m_mapSpherePrimitve)
		{
			auto material = m_mapMaterial.find(sphere.second->GetMaterialID());
			bDirty |= material->second->GetDirty();
			bDirty |= sphere.second->GetDirty();
			if (bDirty)
			{
				break;
			}
		}
	}

	if (bDirty)
	{
		ResetFrameIndex();
	}
}

void RayTracingScene::ResetDirty(Ref<RayTracingCamera>& spCamera)
{
	spCamera->ResetDirty();
	for (auto& sphere : m_mapSpherePrimitve)
	{
		auto material = m_mapMaterial.find(sphere.second->GetMaterialID());
		material->second->ResetDirty();
		sphere.second->ResetDirty();
	}
}

SAND_TABLE_NAMESPACE_END
