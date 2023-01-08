#include "pch.h"
#include "SceneHierarchyPanel.h"
#include "SandTable/Scene/Scene.h"

#include <imgui.h>
SAND_TABLE_NAMESPACE_BEGIN

SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& spScene):
	m_spScene(spScene),
	m_spSelectedEntity(CreateRef<Entity>())
{

}

void SceneHierarchyPanel::OnImGuiRender()
{
	ImGui::Begin("Scene Hierarchy");
	auto spRegistry = m_spScene->Registry();
	spRegistry->each([&](auto entity)
		{
			DrawEntityNode(CreateRef<Entity>(spRegistry, CreateRef<entt::entity>(entity)));
		});
	ImGui::End();

	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
	{
		m_spSelectedEntity = CreateRef<Entity>();
	}

	ImGui::Begin("Properties");
	if (m_spSelectedEntity != nullptr)
	{
		DrawComponent(m_spSelectedEntity);
	}
	ImGui::End();
}

void SceneHierarchyPanel::SetSelectedEntity(const Ref<Entity>& spEntity)
{
	m_spSelectedEntity = spEntity;
}

void SceneHierarchyPanel::DrawEntityNode(const Ref<Entity>& spEntity)
{
	auto tagComponent = spEntity->GetComponent<TagComponent>();
	ImGuiTreeNodeFlags flags = (*m_spSelectedEntity == *spEntity ? ImGuiTreeNodeFlags_Selected : 0)
		| ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	bool bOpened = ImGui::TreeNodeEx((void*)(uint32_t)(*spEntity), flags, tagComponent.Tag.c_str());
	if (ImGui::IsItemClicked())
	{
		m_spSelectedEntity = spEntity;
	}

	if (bOpened)
	{
		ImGui::TreePop();
	}


}

void SceneHierarchyPanel::DrawComponent(const Ref<Entity>& spEntity)
{
	if (spEntity->HasComponent<TagComponent>())
	{
		auto& tagComponent = spEntity->GetComponent<TagComponent>();
		char buffer[UCHAR_MAX]{ 0 };
		strcpy_s(buffer, sizeof(buffer), tagComponent.Tag.c_str());
		if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
		{
			tagComponent.Tag = std::string(buffer);
		}
	}

	if (spEntity->HasComponent<TransformComponent>())
	{
		if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"));
		{
			auto& transformComponent = spEntity->GetComponent<TransformComponent>();
			ImGui::DragFloat3("Position", glm::value_ptr(transformComponent.Transform[3]), 0.1f);
			ImGui::TreePop();
		}
	}

	if (spEntity->HasComponent<CameraComponent>())
	{
		if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"));
		{
			auto& cameraComponent = spEntity->GetComponent<CameraComponent>();

			auto sCurrentProjection = ProjectionTypeStrings[static_cast<unsigned int>(cameraComponent.Camera->GetProjectionType())];

			if (ImGui::BeginCombo("Projection", sCurrentProjection.c_str()))
			{
				for (size_t i = 0; i < ProjectionTypeStrings.size(); i++)
				{
					bool bSelected = sCurrentProjection == ProjectionTypeStrings[i];
					if (ImGui::Selectable(ProjectionTypeStrings[i].c_str(),bSelected))
					{
						sCurrentProjection = ProjectionTypeStrings[i];
						cameraComponent.Camera->SetProjectionType(static_cast<ProjectionType>(i));
					}
					if (bSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			switch (cameraComponent.Camera->GetProjectionType())
			{
			case SandTable::ProjectionType::Perspective:
				break;
			case SandTable::ProjectionType::Orthographic:
				break;
			default:
				break;
			}

			ImGui::TreePop();
		}
	}
}

SAND_TABLE_NAMESPACE_END


