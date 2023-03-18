#include "pch.h"
#include "RayTracingRenderImage.h"

SAND_TABLE_NAMESPACE_BEGIN

Ref<Image> RayTracingRenderImage::m_spImage = nullptr;
void RayTracingRenderImage::Init()
{
	m_spImage = CreateRef<Image>();
}

void RayTracingRenderImage::OnResize(unsigned int uiWidth, unsigned int uiHeight)
{
	if (uiWidth != m_spImage->GetWidth() || uiHeight != m_spImage->GetHeight())
	{
		m_spImage->Resize(uiWidth, uiHeight);
	}
}

void RayTracingRenderImage::BeginScene(const Ref<RayTracingCamera>& spCamera)
{
	auto spRay = CreateRef<Ray>();
	spRay->Origin = spCamera->GetPosition();
	const auto& matView = spCamera->GetInverseView();
	const auto& matProjection = spCamera->GetInverseProjection();

	auto width = m_spImage->GetWidth();
	auto height = m_spImage->GetHeight();

	auto pImageData = m_spImage->GetImageData();

	for (uint32_t y = 0; y < height; y++)
	{
		for (uint32_t x = 0; x < width; x++)
		{
			glm::vec2 vec2Coord = { static_cast<float>(x) / static_cast<float>(width),
				static_cast<float>(y) / static_cast<float>(height) };
			vec2Coord = vec2Coord * 2.f - 1.f; //(0~1)->(-1,1)
			glm::vec4 target = matProjection * glm::vec4(vec2Coord.x, vec2Coord.y, 1, 1);
			spRay->Direction = glm::vec3(matView * glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0)); // World space
			pImageData[x + y * width] = Image::ConvertToRGBA(TraceRay(spRay));
		}
	}
	m_spImage->UpdateImage();
}

void RayTracingRenderImage::EndScene()
{
}

uint32_t RayTracingRenderImage::GetImage()
{
	return m_spImage->GetImage();
}

glm::vec4 RayTracingRenderImage::TraceRay(const Ref<Ray>& spRay)
{
	float fRadius(0.5f);
	glm::vec3 origin = spRay->Origin - glm::vec3(0.5f, 0.0f, 0.0f);
	float fA = glm::dot(spRay->Direction, spRay->Direction);
	float fB = 2.f * glm::dot(origin, spRay->Direction);
	float fC = glm::dot(origin, origin) - fRadius * fRadius;

	float discriminant = fB * fB - 4.0f * fA * fC;
	if (discriminant < 0.f)
	{
		return glm::vec4(0.f, 0.f, 0.f, 1.0f);
	}

	float fT0 = (-fB - glm::sqrt(discriminant)) / (2.f * fA);
	float fT1 = (-fB + glm::sqrt(discriminant)) / (2.f * fA);

	glm::vec3 vec3HitPoint = origin + spRay->Direction * fT0;
	glm::vec3 vec3Normal = glm::normalize(vec3HitPoint);

	glm::vec3 lightDir = glm::normalize(glm::vec3(-1.f));
	float diffuse = glm::max(glm::dot(vec3Normal, -lightDir), 0.f);

	glm::vec3 vec3SphereColor(1.f, 0.f, 1.f);
	vec3SphereColor *= diffuse;
	return glm::vec4(vec3SphereColor, 1.f);
}

SAND_TABLE_NAMESPACE_END