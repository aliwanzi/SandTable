#include "pch.h"
#include "Scene.h"
#include "SandTable/Render/Render2D.h"
#include "SandTable/Scene/Entity.h"

#include "SandTable/Scripting/ScriptEngine.h"

SAND_TABLE_NAMESPACE_BEGIN

namespace
{
	template<typename Component>
	void CopyComponent(Ref<entt::registry> spSrcRegistry, std::unordered_map<UUID, Ref<Entity>>& mapEntity)
	{
		auto srcEntity = spSrcRegistry->view<Component>();
		for (auto entity : srcEntity)
		{
			auto srcUUID = spSrcRegistry->get<IDComponent>(entity).ID;
			auto spEntity = mapEntity.at(srcUUID);

			auto& srcComponent = spSrcRegistry->get<Component>(entity);
			spEntity->AddOrReplaceComponent<Component>(srcComponent);
		}
	}

	template<typename Component>
	void CopyComponentIfExists(Ref<Entity> spSrcEntity, Ref<Entity> spDstEntity)
	{
		if (spSrcEntity->HasComponent<Component>())
		{
			spDstEntity->AddOrReplaceComponent<Component>(spSrcEntity->GetComponent<Component>());
		}
	}
}

Scene::Scene():
	m_spRegistry(CreateRef<entt::registry>()),
	m_uiWidth(0),
	m_uiHeight(0),
	m_spPhysicsSystem2D(nullptr)
{
	LOG_DEV_INFO("Scene Creat");
}

Scene::~Scene()
{
	LOG_DEV_INFO("Scene Destroy");
}

Scene::Scene(const Ref<Scene>& spScene):
	m_spRegistry(CreateRef<entt::registry>()),
	m_uiWidth(spScene->m_uiWidth),
	m_uiHeight(spScene->m_uiHeight),
	m_spPhysicsSystem2D(nullptr)
{
	auto spSrcRegistry = spScene->m_spRegistry;
	auto entity = spSrcRegistry->view<IDComponent>();
	m_mapEntity.clear();

	for (auto e : entity)
	{
		const auto& uuid = spSrcRegistry->get<IDComponent>(e).ID;
		const auto& name = spSrcRegistry->get<TagComponent>(e).Tag;
		auto spNewEntity = CreateEntityWithUUID(uuid, name);
		m_mapEntity[uuid] = spNewEntity;
	}

	CopyComponent<TransformComponent>(spSrcRegistry, m_mapEntity);
	CopyComponent<SpriteRenderComponent>(spSrcRegistry, m_mapEntity);
	CopyComponent<CircleRenderComponent>(spSrcRegistry, m_mapEntity);
	CopyComponent<CameraComponent>(spSrcRegistry, m_mapEntity);
	CopyComponent<RigidBody2DComponent>(spSrcRegistry, m_mapEntity);
	CopyComponent<BoxCollider2DComponent>(spSrcRegistry, m_mapEntity);
	CopyComponent<CircleCollider2DComponent>(spSrcRegistry, m_mapEntity);
	CopyComponent<ScriptComponent>(spSrcRegistry, m_mapEntity);
}

Ref<Entity> Scene::CreateEntity(const Ref<Entity>& spSrcEntity)
{
	SAND_TABLE_ASSERT(spSrcEntity,"Source Entitiy is null CreateEntity Scene")
	auto spDstEntity = CreateEntity(spSrcEntity->GetName());
	CopyComponentIfExists<TransformComponent>(spSrcEntity, spDstEntity);
	CopyComponentIfExists<SpriteRenderComponent>(spSrcEntity, spDstEntity);
	CopyComponentIfExists<CircleRenderComponent>(spSrcEntity, spDstEntity);
	CopyComponentIfExists<CameraComponent>(spSrcEntity, spDstEntity);
	CopyComponentIfExists<RigidBody2DComponent>(spSrcEntity, spDstEntity);
	CopyComponentIfExists<BoxCollider2DComponent>(spSrcEntity, spDstEntity);
	CopyComponentIfExists<CircleCollider2DComponent>(spSrcEntity, spDstEntity);
	CopyComponentIfExists<ScriptComponent>(spSrcEntity, spDstEntity);
	return spDstEntity;
}

Ref<Entity> Scene::CreateEntity(const std::string& sName)
{
	return CreateEntityWithUUID(UUID(), sName);
}

Ref<Entity> Scene::CreateEntityWithUUID(const UUID& uuid, const std::string& sName)
{
	auto spEntity = CreateRef<Entity>(m_spRegistry);
	spEntity->AddComponent<IDComponent>(uuid);
	spEntity->AddComponent<TransformComponent>();
	spEntity->AddComponent<TagComponent>(sName.empty() ? "Entity" : sName);

	m_mapEntity[uuid] = spEntity;

	return spEntity;
}

void Scene::RemoveEntity(Ref<Entity> spEntity)
{
	m_mapEntity.erase(spEntity->GetUUID());
	spEntity->Destrory();
}

void Scene::OnRuntimeStart()
{
	OnPhysics2DStart();
	OnScriptStart();
}

void Scene::OnRuntimeStop()
{
	OnPhysics2DStop();
	OnScriptStop();
}

void Scene::OnSimulationStart()
{
	OnPhysics2DStart();
}

void Scene::OnSimulationStop()
{
	OnPhysics2DStop();
}

void Scene::OnPhysics2DStart()
{
	m_spPhysicsSystem2D = CreateRef<PhysicsSystem2D>();
	auto components = m_spRegistry->view<RigidBody2DComponent>();
	for (auto component : components)
	{
		auto spEntity = CreateRef<Entity>(m_spRegistry, component);
		auto& transform = spEntity->GetComponent<TransformComponent>();
		auto& rigidBody2D = spEntity->GetComponent<RigidBody2DComponent>();

		m_spPhysicsSystem2D->CreateBody(rigidBody2D, transform);

		if (spEntity->HasComponent<BoxCollider2DComponent>())
		{
			auto& boxCollider2D = spEntity->GetComponent<BoxCollider2DComponent>();
			m_spPhysicsSystem2D->CreatePolygonShape(boxCollider2D, transform);
		}

		if (spEntity->HasComponent<CircleCollider2DComponent>())
		{
			auto& circleCollider2D = spEntity->GetComponent<CircleCollider2DComponent>();

			m_spPhysicsSystem2D->CreateCircleShape(circleCollider2D, transform);
		}
	}
}

void Scene::OnPhysics2DStop()
{
	m_spPhysicsSystem2D = nullptr;
}

void Scene::OnScriptStart()
{
	ScriptEngine::OnRuntimeStart(shared_from_this());
	auto view = m_spRegistry->view<ScriptComponent>();
	for (auto component : view)
	{
		auto spEntity = CreateRef<Entity>(m_spRegistry, component);
		ScriptEngine::OnCreateEntity(spEntity);
	}
}

void Scene::OnScriptStop()
{
	ScriptEngine::OnRuntimeStop();
}

void Scene::OnShowPhysicsCollider(const Ref<Camera>& spCamera)
{
	Render2D::BeginScene(spCamera);
	{
		auto view = m_spRegistry->view<TransformComponent, CircleCollider2DComponent>();
		for (auto entity : view)
		{
			const auto [tc, cc2d] = view.get<TransformComponent, CircleCollider2DComponent>(entity);
			auto spCirclePrimitive = CreateRef<CirclePrimitive>();
			spCirclePrimitive->SetColor(glm::vec4(0.f, 1.f, 0.f, 1.f));
			spCirclePrimitive->SetThickness(0.05f);

			glm::vec3 translation = tc.Translation + glm::vec3(cc2d.Offset, 0.001f);
			glm::vec3 scale = tc.Scale * glm::vec3(cc2d.Radius * 2.f + 0.05f);
			glm::mat4 transform = glm::translate(glm::mat4(1.f), translation) * glm::scale(glm::mat4(1.f), scale);

			Render2D::DrawPrimitive(transform, spCirclePrimitive);
		}
	}
	{
		auto view = m_spRegistry->view<TransformComponent, BoxCollider2DComponent>();
		for (auto entity : view)
		{
			const auto [tc, bc2d] = view.get<TransformComponent, BoxCollider2DComponent>(entity);

			auto  spQuadPrimitive = CreateRef<QuadPrimitive>();
			spQuadPrimitive->SetColor(glm::vec4(0.f, 1.f, 0.f, 1.f));
	
			glm::vec3 translation = tc.Translation + glm::vec3(bc2d.Offset, 0.001f);
			glm::vec3 scale = tc.Scale * glm::vec3(bc2d.Size * 2.f, 1.f);

			glm::mat4 transform = glm::translate(glm::mat4(1.f), translation)
				* glm::rotate(glm::mat4(1.f), tc.Rotation.z, glm::vec3(0.f, 0.f, 1.f))
				* glm::scale(glm::mat4(1.f), scale);

			Render2D::DrawBoundary(transform, spQuadPrimitive);
		}
	}
	Render2D::EndScene();
}

void Scene::OnUpdate(const TimeStep& timeStep)
{
	{
		auto view = m_spRegistry->view<ScriptComponent>();
		for (auto component : view)
		{
			auto spEntity = CreateRef<Entity>(m_spRegistry, component);
			ScriptEngine::OnUpdateEntity(spEntity, timeStep);
		}
	}

	{
		m_spPhysicsSystem2D->OnUpdate(timeStep);

		auto components = m_spRegistry->view<RigidBody2DComponent>();
		for (auto component : components)
		{
			auto spEntity = CreateRef<Entity>(m_spRegistry, component);
			auto& transform = spEntity->GetComponent<TransformComponent>();
			auto& rigidBody2D = spEntity->GetComponent<RigidBody2DComponent>();

			m_spPhysicsSystem2D->UpdateSystem(rigidBody2D, transform);
		}
	}

	{
		auto cameraEntity = m_spRegistry->view<TransformComponent, CameraComponent>();
		for (auto component : cameraEntity)
		{
			const auto [cameraTransform, cameraComponent] = cameraEntity.get<TransformComponent, CameraComponent>(component);
			if (cameraComponent.Primary)
			{
				auto camera = cameraComponent.GetCamera();
				camera->SetViewMatrix(cameraTransform.GetTransform());
				RenderScene(camera);
				break;
			}
		}
	}

}

void Scene::OnUpdate(const Ref<Camera>& spCamera)
{
	RenderScene(spCamera);
}

void Scene::OnUpdate(const TimeStep& timeStep, const Ref<Camera>& spCamera)
{
	{
		m_spPhysicsSystem2D->OnUpdate(timeStep);

		auto components = m_spRegistry->view<RigidBody2DComponent>();
		for (auto component : components)
		{
			auto spEntity = CreateRef<Entity>(m_spRegistry, component);
			auto& transform = spEntity->GetComponent<TransformComponent>();
			auto& rigidBody2D = spEntity->GetComponent<RigidBody2DComponent>();

			m_spPhysicsSystem2D->UpdateSystem(rigidBody2D, transform);
		}
	}

	RenderScene(spCamera);
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
			if (!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.PerspecCamera->SetViewPortSize(uiWidth, uiHeight);
				cameraComponent.OrthoCamera->SetViewPortSize(uiWidth, uiHeight);
			}
		}
	}
}

const Ref<entt::registry>& Scene::Registry() const
{
	return m_spRegistry;
}

Ref<Entity> Scene::GetPrimaryCameraEntity()
{
	auto cameraView = m_spRegistry->view<CameraComponent>();
	for (auto entity : cameraView)
	{
		const auto& cameraComponent = cameraView.get<CameraComponent>(entity);
		if (cameraComponent.Primary)
		{
			return CreateRef<Entity>(m_spRegistry, entity);
		}
	}
	return nullptr;
}

Ref<Entity> Scene::GetEntityByUUID(UUID uiEntityID)
{
	if (m_mapEntity.find(uiEntityID) != m_mapEntity.end())
	{
		return m_mapEntity[uiEntityID];
	}
	else
	{
		return nullptr;
	}
}

void Scene::RenderScene(const Ref<Camera>& spCamera)
{
	Render2D::BeginScene(spCamera);

	auto spriteView = m_spRegistry->view<TransformComponent, SpriteRenderComponent>();
	for (auto spriteComponent : spriteView)
	{
		const auto [spriteTransform, sprite] = spriteView.get<TransformComponent, SpriteRenderComponent>(spriteComponent);

		Render2D::DrawSprite(spriteTransform.GetTransform(), sprite, static_cast<int>(spriteComponent));
	}

	auto circleView = m_spRegistry->view<TransformComponent, CircleRenderComponent>();
	for (auto circleComponent : circleView)
	{
		const auto [circleTransform, circle] = circleView.get<TransformComponent, CircleRenderComponent>(circleComponent);

		Render2D::DrawCircle(circleTransform.GetTransform(), circle, static_cast<int>(circleComponent));
	}


	Render2D::EndScene();
}


SAND_TABLE_NAMESPACE_END
