#include "pch.h"
#include "RayTracingRenderImage.h"
#include <omp.h>

SAND_TABLE_NAMESPACE_BEGIN

Ref<RayTracingRenderImage::RenderStroge> RayTracingRenderImage::m_spRenderStroge = nullptr;
void RayTracingRenderImage::Init()
{
	m_spRenderStroge = CreateRef<RenderStroge>();
	m_spRenderStroge->spImage = CreateRef<Image>();
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

void RayTracingRenderImage::RenderPrimitve(const MapSphere& vecSpherePrimitive)
{
	if (vecSpherePrimitive.empty())
	{
		return;
	}
	auto width = m_spRenderStroge->spImage->GetWidth();
	auto height = m_spRenderStroge->spImage->GetHeight();

	auto pImageData = m_spRenderStroge->spImage->GetImageData();

	for (uint32_t y = 0; y < height; y++)
	{
		for (uint32_t x = 0; x < width; x++)
		{
			glm::vec2 vec2Coord = { static_cast<float>(x) / static_cast<float>(width),
				static_cast<float>(y) / static_cast<float>(height) };
			vec2Coord = vec2Coord * 2.f - 1.f; //(0~1)->(-1,1)
			glm::vec4 target = m_spRenderStroge->matProjection * glm::vec4(vec2Coord.x, vec2Coord.y, 1, 1);
			m_spRenderStroge->spRay->Direction = glm::vec3(m_spRenderStroge->matView
				* glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0));

			pImageData[x + y * width] = Image::ConvertToRGBA(PerPixel(m_spRenderStroge->spRay, vecSpherePrimitive));
		}
	}
}

uint32_t RayTracingRenderImage::GetImage()
{
	return m_spRenderStroge->spImage->GetImage();
}

glm::vec4 RayTracingRenderImage::PerPixel(Ref<Ray> ray, const MapSphere& mapSpherePrimitive)
{
	glm::vec3 vec3Color(0.f);
	int fBounces(2);
	float fMultiplier = 1.f;

	auto spRay = CreateRef<Ray>();
	spRay->Origin = ray->Origin;
	spRay->Direction = ray->Direction;

	for (int i = 0; i < fBounces; i++)
	{
		auto spHitPayLoad = TraceRay(spRay, mapSpherePrimitive);
		if (spHitPayLoad == nullptr)
		{
			break;
		}

		glm::vec3 lightDir = glm::normalize(glm::vec3(-1.f));
		float diffuse = glm::max(glm::dot(spHitPayLoad->WorldNormal, -lightDir), 0.f);

		const auto& spSphere = mapSpherePrimitive.find(spHitPayLoad->EntityID)->second;
		glm::vec3 vec3SphereColor(spSphere->GetMaterial()->GetAlbedo());
		vec3SphereColor *= diffuse;

		vec3Color += vec3SphereColor * fMultiplier;
		fMultiplier *= 0.7f;

		spRay->Origin = spHitPayLoad->WorldPosition + spHitPayLoad->WorldNormal * 0.0001f;
		spRay->Direction = glm::reflect(spRay->Direction, spHitPayLoad->WorldNormal);
	}

	return glm::vec4(vec3Color, 1.f);
}

Ref<HitPayLoad> RayTracingRenderImage::TraceRay(const Ref<Ray>& spRay, const MapSphere& mapSpherePrimitive)
{
	float fHitDistance = std::numeric_limits<float>::max();
	Ref<SpherePrimitive> spHitSphere(nullptr);
	for (const auto& iter : mapSpherePrimitive)
	{
		const auto& sphere = iter.second;
		glm::vec3 origin = spRay->Origin - sphere->GetPosition();

		float fA = glm::dot(spRay->Direction, spRay->Direction);
		float fB = 2.f * glm::dot(origin, spRay->Direction);
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
			spHitSphere = sphere;
		}
	}

	if (spHitSphere != nullptr)
	{
		auto spHitPayLoad = CreateRef<HitPayLoad>();

		glm::vec3 origin = spRay->Origin - spHitSphere->GetPosition();

		spHitPayLoad->HitDistance = fHitDistance;
		spHitPayLoad->WorldPosition = spRay->Origin + spRay->Direction * fHitDistance;
		spHitPayLoad->WorldNormal = glm::normalize(spHitPayLoad->WorldPosition - spHitSphere->GetPosition());
		spHitPayLoad->EntityID = spHitSphere->GetEntityID();
		return spHitPayLoad;
	}
	return nullptr;
}

SAND_TABLE_NAMESPACE_END