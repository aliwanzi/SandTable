#include "pch.h"
#include "RayTracingScene.h"
#include "SandTable/Math/MathUtils.h"
#include <execution>

SAND_TABLE_NAMESPACE_BEGIN

RayTracingScene::RayTracingScene() :
	m_bAccumulate(true),
	m_iFrameIndex(1),
	m_spImage(CreateRef<Image>()),
	m_spAccumulateBuffer(CreateRef<DataBuffer>(0, sizeof(glm::vec4) / sizeof(uint8_t))),
	m_spObjectContainer(nullptr),
	m_vec3BackGroundColor(glm::dvec3(0.0))
{
}

void RayTracingScene::OnUpdate(const TimeStep& timeStep, Ref<RayTracingCamera>& spCamera)
{
	if (m_spObjectContainer == nullptr || m_mapMaterial.empty())
	{
		return;
	}

	PreRender(spCamera);
	Render(spCamera);
	PostRender(spCamera);
}

void RayTracingScene::OnViewPortResize(unsigned int uiWidth, unsigned int uiHeight)
{
	if (uiWidth != m_spImage->GetWidth()
		|| uiHeight != m_spImage->GetHeight())
	{
		LOG_DEV_INFO("RayTracingRenderImage::OnResize");
		m_spImage->Resize(uiWidth, uiHeight);
		m_spAccumulateBuffer->Resize(uiWidth * uiHeight);

		m_vecImageHorizontalInter.resize(uiWidth);
		m_vecImageVerticalIter.resize(uiHeight);
		for (uint32_t i = 0; i < uiWidth; i++)
		{
			m_vecImageHorizontalInter[i] = i;
		}
		for (uint32_t i = 0; i < uiHeight; i++)
		{
			m_vecImageVerticalIter[i] = i;
		}
	}
}

void RayTracingScene::SetObjectContainer(std::shared_ptr<ObjectContainer> spObjectContainer)
{
	m_spObjectContainer = spObjectContainer;
}

const std::shared_ptr<ObjectContainer>& RayTracingScene::GetObjectContainer()const
{
	return m_spObjectContainer;
}

void RayTracingScene::AddMaterial(const Ref<Material>& spMaterial)
{
	m_mapMaterial[spMaterial->GetMaterialID()] = spMaterial;
}

void RayTracingScene::ClearMaterial()
{
	m_mapMaterial.clear();
}

MapMaterial& RayTracingScene::GetMaterials()
{
	return m_mapMaterial;
}

void RayTracingScene::SetBackGroundColor(const glm::dvec3& background)
{
	m_vec3BackGroundColor = background;
}

const glm::dvec3& RayTracingScene::GetBackGroundColor() const
{
	return m_vec3BackGroundColor;
}

uint32_t RayTracingScene::GetRenderImage() const
{
	return m_spImage->GetImage();
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

void RayTracingScene::PreRender(Ref<RayTracingCamera>& spCamera)
{
	bool bDirty(spCamera->GetDirty());
	if (!bDirty)
	{
		const auto& mapObject = m_spObjectContainer->GetAllObject();
		for (auto& sphere : mapObject)
		{
			auto material = m_mapMaterial.find(sphere->GetMaterialID());
			bDirty |= material->second->GetDirty();
			bDirty |= sphere->GetDirty();
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

void RayTracingScene::Render(Ref<RayTracingCamera>& spCamera)
{
	if (m_iFrameIndex == 1)
	{
		m_spAccumulateBuffer->Clear();
	}
	auto width = m_spImage->GetWidth();
	auto height = m_spImage->GetHeight();
	auto pImageData = m_spImage->GetImageData();
	auto pAccumulateBuffer = m_spAccumulateBuffer->As<glm::vec4>();

#define MultiThread
#ifdef MultiThread
	std::for_each(std::execution::par, m_vecImageVerticalIter.begin(), m_vecImageVerticalIter.end(),
		[&](uint32_t y)
		{
			std::for_each(m_vecImageHorizontalInter.begin(), m_vecImageHorizontalInter.end(),
			[&](uint32_t x)
				{
					Ray ray = spCamera->GenRay(x, y);
					auto color = glm::vec4(TraceRay(ray, m_spObjectContainer, 50), 1.f);

					pAccumulateBuffer[x + y * width] += color;

					color = pAccumulateBuffer[x + y * width];
					color /= m_iFrameIndex;

					pImageData[x + y * width] = Image::ConvertToRGBA(color);
				});
		});
#else
	for (uint32_t y = 0; y < height; y++)
	{
		for (uint32_t x = 0; x < width; x++)
		{
			Ray ray = spCamera->GenRay(x, y);
			auto color = glm::vec4(TraceRay(ray, m_spObjectContainer, 50), 1.f);

			pAccumulateBuffer[x + y * width] += color;

			color = pAccumulateBuffer[x + y * width];
			color /= m_iFrameIndex;

			pImageData[x + y * width] = Image::ConvertToRGBA(color);
		}
	}
#endif // MultiThread

	m_bAccumulate ? m_iFrameIndex++ : m_iFrameIndex = 1;
}

void RayTracingScene::PostRender(Ref<RayTracingCamera>& spCamera)
{
	spCamera->ResetDirty();
	const auto& mapObject = m_spObjectContainer->GetAllObject();
	for (auto& sphere : mapObject)
	{
		auto material = m_mapMaterial.find(sphere->GetMaterialID());
		material->second->ResetDirty();
		sphere->ResetDirty();
	}

	m_spImage->UpdateImage();
}


glm::dvec3 RayTracingScene::TraceRay(const Ray& ray, const std::shared_ptr<Hittable>& spHittable, int depth)
{
	if (depth <= 0)
	{
		return glm::dvec3(0.0);
	}
	
	HitRecord rec;
	if (!spHittable->Hit(ray, 0.001, std::numeric_limits<float>::max(), rec))
	{
		return m_vec3BackGroundColor;
	}

	auto material = m_mapMaterial.find(rec.MaterialID);
	if (material == m_mapMaterial.end())
	{
		LOG_DEV_ERROR("Can't find material in TraceRay");
		return glm::dvec3(0.0);
	}

	auto emmitted = material->second->Emitted(rec.WorldPosition, rec.UV);

	Ray scattered;
	glm::dvec3 attenuation;
	if (!material->second->Scatter(ray, rec, attenuation, scattered))
	{
		return emmitted;
	}

	return emmitted + attenuation * TraceRay(scattered, spHittable, depth - 1);
}

SAND_TABLE_NAMESPACE_END
