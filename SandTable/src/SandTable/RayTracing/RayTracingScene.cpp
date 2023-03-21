#include "pch.h"
#include "RayTracingScene.h"
#include "SandTable/Math/Random.h"
#include <execution>

SAND_TABLE_NAMESPACE_BEGIN
RayTracingScene::RayTracingScene() :
	m_bAccumulate(false),
	m_iFrameIndex(1),
	m_spRay(CreateRef<Ray>()),
	m_spImage(CreateRef<Image>()),
	m_spAccumulateBuffer(CreateRef<DataBuffer>(0, sizeof(glm::vec4) / sizeof(uint8_t)))
{

}

void RayTracingScene::OnUpdate(const TimeStep& timeStep, Ref<RayTracingCamera>& spCamera)
{
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

	m_spRay->Origin = spCamera->GetPosition();
	m_matView = spCamera->GetInverseView();
	m_matProjection = spCamera->GetInverseProjection();
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

	std::for_each(std::execution::par, m_vecImageVerticalIter.begin(), m_vecImageVerticalIter.end(),
		[&](uint32_t y)
		{
			std::for_each(m_vecImageHorizontalInter.begin(), m_vecImageHorizontalInter.end(),
				[&](uint32_t x)
				{
					auto color = PerPixel(m_spRay->Origin, x, y);

					pAccumulateBuffer[x + y * width] += color;

					color = pAccumulateBuffer[x + y * width];
					color /= m_iFrameIndex;

					pImageData[x + y * width] = Image::ConvertToRGBA(color);
				});
		});

	//auto width = m_spImage->GetWidth();
	//auto height = m_spImage->GetHeight();
	//for (uint32_t y = 0; y < height; y++)
	//{
	//	for (uint32_t x = 0; x < width; x++)
	//	{
	//		glm::vec2 vec2Coord = { static_cast<float>(x) / static_cast<float>(width),static_cast<float>(y) / static_cast<float>(height) };
	//		vec2Coord = vec2Coord * 2.f - 1.f; //(0~1)->(-1,1)
	//		glm::vec4 target = m_matProjection * glm::vec4(vec2Coord.x, vec2Coord.y, 1, 1);
	//		m_spRay->Direction = glm::vec3(m_matView * glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0));

	//		auto color = PerPixel(*m_spRay, 50);

	//		m_spAccumulateBuffer->As<glm::vec4>()[x + y * width] += color;

	//		color = m_spAccumulateBuffer->As<glm::vec4>()[x + y * width];
	//		color /= m_iFrameIndex;

	//		m_spImage->GetImageData()[x + y * width] = Image::ConvertToRGBA(color);
	//	}
	//}

	m_bAccumulate ? m_iFrameIndex++ : m_iFrameIndex = 1;
}

void RayTracingScene::PostRender(Ref<RayTracingCamera>& spCamera)
{
	spCamera->ResetDirty();
	for (auto& sphere : m_mapSpherePrimitve)
	{
		auto material = m_mapMaterial.find(sphere.second->GetMaterialID());
		material->second->ResetDirty();
		sphere.second->ResetDirty();
	}

	m_spImage->UpdateImage();
}

glm::vec4 RayTracingScene::PerPixel(const glm::vec3& rayOrigin, uint32_t uiX, uint32_t uiY)
{
	auto width = m_spImage->GetWidth();
	auto height =m_spImage->GetHeight();

	Ray ray;
	ray.Origin = rayOrigin;
	glm::vec2 vec2Coord = { static_cast<float>(uiX) / static_cast<float>(width),static_cast<float>(uiY) / static_cast<float>(height) };
	vec2Coord = vec2Coord * 2.f - 1.f; //(0~1)->(-1,1)
	glm::vec4 target = m_matProjection * glm::vec4(vec2Coord.x, vec2Coord.y, 1, 1);
	ray.Direction = glm::vec3(m_matView * glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0));

	glm::vec3 vec3Color(0.f);
	glm::vec3 vec3SkyColor(0.6f, 0.7f, 0.9f);
	int fBounces(50);
	float fMultiplier = 1.f;

	HitPayLoad hitPayLoad;
	for (int i = 0; i < fBounces; i++)
	{
		TraceRay(ray, hitPayLoad);
		if (hitPayLoad.HitDistance < 0)
		{
			vec3Color += vec3SkyColor * fMultiplier;
			break;
		}

		glm::vec3 lightDir = glm::normalize(glm::vec3(-1.f));
		float diffuse = glm::max(glm::dot(hitPayLoad.WorldNormal, -lightDir), 0.f);

		const auto& sphere = m_mapSpherePrimitve.find(hitPayLoad.EntityID)->second;
		const auto& material = m_mapMaterial.find(sphere->GetMaterialID())->second;
		glm::vec3 vec3SphereColor;
		material->Scatter(hitPayLoad, ray, vec3SphereColor);
		vec3SphereColor *= diffuse;

		vec3Color += vec3SphereColor * fMultiplier;
		fMultiplier *= 0.5f;
	}

	return glm::vec4(vec3Color, 1.f);
}

void RayTracingScene::TraceRay(const Ray& ray,HitPayLoad& hitPayLoad)
{
	float fHitDistance = std::numeric_limits<float>::max();
	uint32_t uiSphereID(-1);
	for (const auto& iter : m_mapSpherePrimitve)
	{
		const auto& sphere = iter.second;
		auto& origin = ray.Origin - sphere->GetPosition();

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

	auto iter = m_mapSpherePrimitve.find(uiSphereID);
	if (iter != m_mapSpherePrimitve.end())
	{
		auto& position = iter->second->GetPosition();
		glm::vec3 origin = ray.Origin - position;

		hitPayLoad.HitDistance = fHitDistance;
		hitPayLoad.WorldPosition = ray.Origin + ray.Direction * fHitDistance;
		hitPayLoad.SetWorldNormal(ray, hitPayLoad.WorldPosition - position);
		hitPayLoad.EntityID = uiSphereID;
	}
	else
	{
		hitPayLoad.HitDistance = -1;
	}
}

SAND_TABLE_NAMESPACE_END
