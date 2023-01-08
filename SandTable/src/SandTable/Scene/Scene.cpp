#include "pch.h"
#include "Scene.h"
#include "SandTable/Render/Render2D.h"
#include "SandTable/Scene/Entity.h"

SAND_TABLE_NAMESPACE_BEGIN

Scene::Scene():
	m_spRegistry(CreateRef<entt::registry>()),
	m_uiWidth(0),
	m_uiHeight(0)
{

}

Scene::~Scene()
{

}

Ref<Entity> Scene::CreateEntity(const std::string& sName)
{
	auto spEntity = CreateRef<Entity>(m_spRegistry);
	spEntity->AddComponent<TransformComponent>();
	spEntity->AddComponent<TagComponent>(sName.empty() ? "Entity" : sName);
	return spEntity;
}

void Scene::OnUpdate(const TimeStep& timeStep)
{
	{
		m_spRegistry->view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
		{
			if (nsc.Instance == nullptr)
			{
				nsc.InstantiateFunction();
				nsc.Instance->SetEntity(CreateRef<Entity>(m_spRegistry, CreateRef<entt::entity>(entity)));
				nsc.Instance->OnCreate();
			}

			nsc.Instance->OnUpdate(timeStep);
		});
	}

	{
		auto cameraView = m_spRegistry->view<TransformComponent, CameraComponent>();
		for (auto cameraComponent : cameraView)
		{
			const auto& [cameraTransform, camera] = cameraView.get<TransformComponent, CameraComponent>(cameraComponent);
			if (camera.Primary)
			{
				Render2D::BeginScene(camera.Camera, cameraTransform.Transform);
				auto spriteView = m_spRegistry->view<TransformComponent, SpriteRenderComponent>();
				for (auto spriteComponent : spriteView)
				{
					const auto& [spriteTransform, sprite] = spriteView.get<TransformComponent, SpriteRenderComponent>(spriteComponent);

					Render2D::DrawQuad(spriteTransform.Transform, sprite.Color);
				}

				Render2D::EndScene();
				break;
			}
		}
	}

}

void Scene::OnViewPortResize(unsigned int uiWidth, unsigned int uiHeight)
{
	if (m_uiWidth != uiWidth || m_uiHeight != uiHeight)
	{
		m_uiWidth = uiWidth;
		m_uiHeight = uiHeight;
		auto cameraView = m_spRegistry->view<CameraComponent>();
		for (auto component : cameraView)
		{
			auto cameraComponent = cameraView.get<CameraComponent>(component);
			if (!cameraComponent.FixedAspectRation)
			{
				auto orthoCamera = std::dynamic_pointer_cast<OrthoGraphicCamera>(cameraComponent.Camera);
				if (orthoCamera != nullptr)
				{
					orthoCamera->SetViewPortSize(uiWidth, uiHeight);
				}
			}
		}
	}
}

const Ref<entt::registry>& Scene::Registry() const
{
	return m_spRegistry;
}

SAND_TABLE_NAMESPACE_END
