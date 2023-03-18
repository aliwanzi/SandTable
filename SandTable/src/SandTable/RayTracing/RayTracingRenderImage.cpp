#include "pch.h"
#include "RayTracingRenderImage.h"
#include "SandTable/Scene/Primitive/SpherePrimitive.h"
#include "SandTable/RayTracing/RayTracingMaterial.h"

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

void RayTracingRenderImage::RenderPrimitve(const std::vector<Ref<SpherePrimitive>>& vecSpherePrimitive)
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

			pImageData[x + y * width] = Image::ConvertToRGBA(TraceRay(vecSpherePrimitive));
		}
	}
}

uint32_t RayTracingRenderImage::GetImage()
{
	return m_spRenderStroge->spImage->GetImage();
}

glm::vec4 RayTracingRenderImage::TraceRay(const std::vector<Ref<SpherePrimitive>>& vecSpherePrimitive)
{
	float fHitDistance = std::numeric_limits<float>::max();
	Ref<SpherePrimitive> spHitSphere(nullptr);
	auto spRay = m_spRenderStroge->spRay;
	for (auto& spSphere : vecSpherePrimitive)
	{
		glm::vec3 origin = spRay->Origin - spSphere->GetPosition();

		float fA = glm::dot(spRay->Direction, spRay->Direction);
		float fB = 2.f * glm::dot(origin, spRay->Direction);
		float fC = glm::dot(origin, origin) - spSphere->GetRadius() * spSphere->GetRadius();

		float discriminant = fB * fB - 4.0f * fA * fC;
		if (discriminant < 0.f)
		{
			continue;
		}
		float fCloset = (-fB - glm::sqrt(discriminant)) / (2.f * fA);
		if (fCloset < fHitDistance)
		{
			fHitDistance = fCloset;
			spHitSphere = spSphere;
		}
	}

	if (spHitSphere == nullptr)
	{
		return glm::vec4(0.f, 0.f, 0.f, 1.f);
	}
	glm::vec3 vec3HitPoint = spRay->Origin + spRay->Direction * fHitDistance;
	glm::vec3 vec3Normal = glm::normalize(vec3HitPoint-spHitSphere->GetPosition());

	glm::vec3 lightDir = glm::normalize(glm::vec3(-1.f));
	float diffuse = glm::max(glm::dot(vec3Normal, -lightDir), 0.f);

	glm::vec3 vec3SphereColor(spHitSphere->GetMaterial()->GetAlbedo());
	vec3SphereColor *= diffuse;
	return glm::vec4(vec3SphereColor, 1.f);
}

SAND_TABLE_NAMESPACE_END