#include "pch.h"
#include "RayTracingRenderImage.h"
#include "SandTable/Math/Random.h"
#include <execution>
SAND_TABLE_NAMESPACE_BEGIN

Ref<RayTracingRenderImage::RenderStroge> RayTracingRenderImage::m_spRenderStroge = nullptr;
void RayTracingRenderImage::Init()
{
	m_spRenderStroge = CreateRef<RenderStroge>();
	m_spRenderStroge->spImage = CreateRef<Image>();
	m_spRenderStroge->spAccumulateBuffer = CreateRef<DataBuffer>(0, sizeof(glm::vec4) / sizeof(uint8_t));
	m_spRenderStroge->spRay = CreateRef<Ray>();
	m_spRenderStroge->matProjection = glm::mat4(1.f);
	m_spRenderStroge->matView = glm::mat4(1.f);
}

void RayTracingRenderImage::OnResize(unsigned int uiWidth, unsigned int uiHeight)
{
	if (uiWidth != m_spRenderStroge->spImage->GetWidth()
		|| uiHeight != m_spRenderStroge->spImage->GetHeight())
	{
		LOG_DEV_INFO("RayTracingRenderImage::OnResize");
		m_spRenderStroge->spImage->Resize(uiWidth, uiHeight);
		m_spRenderStroge->spAccumulateBuffer->Resize(uiWidth * uiHeight);

		m_spRenderStroge->vecImageHorizontalInter.resize(uiWidth);
		m_spRenderStroge->vecImageVerticalIter.resize(uiHeight);
		for (uint32_t i = 0; i < uiWidth; i++)
		{
			m_spRenderStroge->vecImageHorizontalInter[i] = i;
		}
		for (uint32_t i = 0; i < uiHeight; i++)
		{
			m_spRenderStroge->vecImageVerticalIter[i] = i;
		}
	}
}

void RayTracingRenderImage::BeginScene(const Ref<RayTracingCamera>& spCamera)
{
	m_spRenderStroge->spRay->Origin = spCamera->GetPosition();
	m_spRenderStroge->matView = spCamera->GetInverseView();
	m_spRenderStroge->matProjection = spCamera->GetInverseProjection();
}

void RayTracingRenderImage::EndScene()
{
	m_spRenderStroge->spImage->UpdateImage();
}

void RayTracingRenderImage::RenderPrimitve(Ref<RayTracingScene>& spRayTracingScene)
{
	auto& iFrameIndex = spRayTracingScene->GetFrameIndex();

	auto width = m_spRenderStroge->spImage->GetWidth();
	auto height = m_spRenderStroge->spImage->GetHeight();
	if (iFrameIndex == 1)
	{
		m_spRenderStroge->spAccumulateBuffer->Clear();
	}

	auto pImageData = m_spRenderStroge->spImage->GetImageData();
	auto pAccumulateBuffer = m_spRenderStroge->spAccumulateBuffer->As<glm::vec4>();

#define MT 1
#if MT
	std::for_each(std::execution::par, m_spRenderStroge->vecImageVerticalIter.begin(), m_spRenderStroge->vecImageVerticalIter.end(),
		[&](uint32_t y)
		{
			std::for_each(m_spRenderStroge->vecImageHorizontalInter.begin(), m_spRenderStroge->vecImageHorizontalInter.end(),
				[&](uint32_t x)
				{
					auto color = PerPixel(m_spRenderStroge->spRay->Origin, x, y, spRayTracingScene);

					pAccumulateBuffer[x + y * width] += color;

					color = pAccumulateBuffer[x + y * width];
					color /= iFrameIndex;

					pImageData[x + y * width] = Image::ConvertToRGBA(color);
				});
				});
#else
	for (uint32_t y = 0; y < height; y++)
	{
		for (uint32_t x = 0; x < width; x++)
		{
			auto color = PerPixel(m_spRenderStroge->spRay->Origin, x, y, spRayTracingScene);

			pAccumulateBuffer[x + y * width] += color;

			color = pAccumulateBuffer[x + y * width];
			color /= iFrameIndex;

			pImageData[x + y * width] = Image::ConvertToRGBA(color);
		}
	}
#endif


	spRayTracingScene->GetAccumulate() ? iFrameIndex++ : iFrameIndex = 1;
}

uint32_t RayTracingRenderImage::GetImage()
{
	return m_spRenderStroge->spImage->GetImage();
}

glm::vec4 RayTracingRenderImage::PerPixel(const glm::vec3& rayOrigin, uint32_t uiX, uint32_t uiY, Ref<RayTracingScene>& spScene)
{
	auto& mapSpherePrimitive = spScene->GetSpherePrimives();
	auto& mapMaterials = spScene->GetMaterials();
	auto width = m_spRenderStroge->spImage->GetWidth();
	auto height = m_spRenderStroge->spImage->GetHeight();

	Ray ray;
	ray.Origin = rayOrigin;
	glm::vec2 vec2Coord = { static_cast<float>(uiX) / static_cast<float>(width),static_cast<float>(uiY) / static_cast<float>(height) };
	vec2Coord = vec2Coord * 2.f - 1.f; //(0~1)->(-1,1)
	glm::vec4 target = m_spRenderStroge->matProjection * glm::vec4(vec2Coord.x, vec2Coord.y, 1, 1);
	ray.Direction = glm::vec3(m_spRenderStroge->matView * glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0));

	glm::vec3 vec3Color(0.f);
	glm::vec3 vec3SkyColor(0.6f, 0.7f, 0.9f);
	int fBounces(5);
	float fMultiplier = 1.f;

	HitPayLoad hitPayLoad;
	for (int i = 0; i < fBounces; i++)
	{
		TraceRay(ray, mapSpherePrimitive, hitPayLoad);
		if (hitPayLoad.HitDistance < 0)
		{
			vec3Color += vec3SkyColor * fMultiplier;
			break;
		}

		glm::vec3 lightDir = glm::normalize(glm::vec3(-1.f));
		float diffuse = glm::max(glm::dot(hitPayLoad.WorldNormal, -lightDir), 0.f);

		const auto& sphere = mapSpherePrimitive.find(hitPayLoad.EntityID)->second;
		const auto& material = mapMaterials.find(sphere->GetMaterialID())->second;
		glm::vec3 vec3SphereColor(material->GetAlbedo());
		vec3SphereColor *= diffuse;

		vec3Color += vec3SphereColor * fMultiplier;
		fMultiplier *= 0.5f;

		ray.Origin = hitPayLoad.WorldPosition + hitPayLoad.WorldNormal * 0.0001f;
		auto vec3Mircor = material->GetRoughness() * Random::Vec3();
		ray.Direction = glm::reflect(ray.Direction, hitPayLoad.WorldNormal + vec3Mircor);
	}

	return glm::vec4(vec3Color, 1.f);
}

void RayTracingRenderImage::TraceRay(const Ray& ray, const MapSphere& mapSpherePrimitive, HitPayLoad& hitPayLoad)
{
	float fHitDistance = std::numeric_limits<float>::max();
	uint32_t uiSphereID(-1);
	for (const auto& iter : mapSpherePrimitive)
	{
		const auto& sphere = iter.second;
		auto&origin = ray.Origin - sphere->GetPosition();

		float fA = glm::dot(ray.Direction, ray.Direction);
		float fB = 2.f * glm::dot(origin, ray.Direction);
		float fC = glm::dot(origin, origin) - sphere->GetRadius() * sphere->GetRadius();

		float discriminant = fB * fB - 4.0f * fA * fC;
		if (discriminant < 0.f)
		{
			continue;
		}
		float fCloset = (-fB - glm::sqrt(discriminant)) / (2.f * fA);
		if (fCloset > 0.f && fCloset < fHitDistance)
		{
			fHitDistance = fCloset;
			uiSphereID = sphere->GetEntityID();
		}
	}

	auto iter = mapSpherePrimitive.find(uiSphereID);
	if (iter!=mapSpherePrimitive.end())
	{
		auto& position = iter->second->GetPosition();
		glm::vec3 origin = ray.Origin - position;

		hitPayLoad.HitDistance = fHitDistance;
		hitPayLoad.WorldPosition = ray.Origin + ray.Direction * fHitDistance;
		hitPayLoad.WorldNormal = glm::normalize(hitPayLoad.WorldPosition - position);
		hitPayLoad.EntityID = uiSphereID;
	}
	else
	{
		hitPayLoad.HitDistance = -1;
	}
}

SAND_TABLE_NAMESPACE_END