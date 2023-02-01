#include "pch.h"
#include "SceneHierarchyPanel.h"
#include "SandTable/Scene/Scene.h"
#include "SandTable/Render/Camera/OrthoGraphicCamera.h"
#include "SandTable/Render/Camera/PerspectiveGraphicCamera.h"
#include "SandTable/Render/Texture/Texture2D.h"
#include "SandTable/Render/Render2D.h"

#include <imgui.h>
#include <imgui_internal.h>

SAND_TABLE_NAMESPACE_BEGIN

extern const std::filesystem::path sAssetsDirector;

namespace 
{
	static void DrawVec3Control(const std::string& sLabel, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(sLabel.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(sLabel.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& sName, const Ref<Entity>& spEntity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (spEntity->HasComponent<T>())
		{
			auto& component = spEntity->GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, sName.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2(lineHeight, lineHeight)))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				spEntity->RemoveComponent<T>();
		}
	}
}

SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& spScene):
	m_spScene(spScene),
	m_spSelectedEntity(nullptr)
{

}

void SceneHierarchyPanel::OnImGuiRender()
{
	ImGui::Begin("Scene Hierarchy");
	auto spRegistry = m_spScene->Registry();
	spRegistry->each([&](auto entity)
		{
			DrawEntityNode(CreateRef<Entity>(spRegistry, entity));
		});

	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
	{
		m_spSelectedEntity = nullptr;
	}

	if (ImGui::BeginPopupContextWindow(0, 1, false))
	{
		if (ImGui::MenuItem("Create Empty Entity"))
		{
			m_spScene->CreateEntity("Empty Entity");
		}

		ImGui::EndPopup();
	}

	ImGui::End();

	ImGui::Begin("Properties");
	if (m_spSelectedEntity != nullptr)
	{
		DrawComponents(m_spSelectedEntity);
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("Add Component");
		}

		if (ImGui::BeginPopup("Add Component"))
		{

			if (!m_spSelectedEntity->HasComponent<CameraComponent>() && ImGui::MenuItem("Camera"))
			{
				m_spSelectedEntity->AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (!m_spSelectedEntity->HasComponent<SpriteRenderComponent>() && ImGui::MenuItem("Sprite Render"))
			{
				m_spSelectedEntity->AddComponent<SpriteRenderComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (!m_spSelectedEntity->HasComponent<RigidBody2DComponent>() && ImGui::MenuItem("Rigid Body2D"))
			{
				m_spSelectedEntity->AddComponent<RigidBody2DComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (!m_spSelectedEntity->HasComponent<BoxCollider2DComponent>() && ImGui::MenuItem("Box Collider2D"))
			{
				m_spSelectedEntity->AddComponent<BoxCollider2DComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

void SceneHierarchyPanel::SetSelectedEntity(const Ref<Entity>& spEntity)
{
	m_spSelectedEntity = spEntity;
}

const Ref<Entity>& SceneHierarchyPanel::GetSelectedEntity() const
{
	return m_spSelectedEntity;
}

void SceneHierarchyPanel::SetSelectedScene(const Ref<Scene>& spScene)
{
	m_spScene = spScene;
	m_spSelectedEntity = nullptr;
}

void SceneHierarchyPanel::DrawEntityNode(const Ref<Entity>& spEntity)
{
	auto tagComponent = spEntity->GetComponent<TagComponent>();
	ImGuiTreeNodeFlags flags = (m_spSelectedEntity != nullptr ? ImGuiTreeNodeFlags_Selected : 0)
		| ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	bool bOpened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)(*spEntity), flags, tagComponent.Tag.c_str());
	if (ImGui::IsItemClicked())
	{
		m_spSelectedEntity = spEntity;
	}

	bool entityDeleted = false;
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Delete Entity"))
		{
			entityDeleted = true;
		}

		ImGui::EndPopup();
	}

	if (bOpened)
	{
		ImGui::TreePop();
	}

	if (entityDeleted)
	{
		m_spScene->DestroyEntity(spEntity);
		if (*m_spSelectedEntity == *spEntity)
			m_spSelectedEntity = nullptr;
	}
}

void SceneHierarchyPanel::DrawComponents(const Ref<Entity>& spEntity)
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

	DrawComponent<TransformComponent>("Transform", spEntity, [](auto& component)
		{
			DrawVec3Control("Translation", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", component.Scale, 1.0f);
		});

	DrawComponent<CameraComponent>("Camera", spEntity, [](auto& component)
		{
			auto sCurrentProjection = ProjectionTypeStrings[static_cast<unsigned int>(component.Projection)];

			if (ImGui::BeginCombo("Projection", sCurrentProjection.c_str()))
			{
				for (size_t i = 0; i < ProjectionTypeStrings.size(); i++)
				{
					bool bSelected = sCurrentProjection == ProjectionTypeStrings[i];
					if (ImGui::Selectable(ProjectionTypeStrings[i].c_str(), bSelected))
					{
						component.Projection = static_cast<ProjectionType>(i);
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			switch (component.Projection)
			{
			case SandTable::ProjectionType::Perspective:
			{
				auto spPerspecCamera = std::dynamic_pointer_cast<PerspectiveGraphicCamera>(component.PerspecCamera);
				SAND_TABLE_ASSERT(spPerspecCamera, "Perspectiv Graphic Camera is null in Scene Hierarchy Panel");

				float fPerFov = spPerspecCamera->GetPerspectiveFOV();
				if (ImGui::DragFloat("FOV", &fPerFov))
				{
					spPerspecCamera->SetPerspectiveFOV(fPerFov);
				}

				float fOrthoNear = spPerspecCamera->GetNearClip();
				if (ImGui::DragFloat("Near", &fOrthoNear))
				{
					spPerspecCamera->SetNearClip(fOrthoNear);
				}

				float fOrthoFar = spPerspecCamera->GetFarClip();
				if (ImGui::DragFloat("Far", &fOrthoFar))
				{
					spPerspecCamera->SetFarClip(fOrthoFar);
				}
				break;
			}
			case SandTable::ProjectionType::Orthographic:
			{
				auto spOrthoCamera = std::dynamic_pointer_cast<OrthoGraphicCamera>(component.OrthoCamera);
				SAND_TABLE_ASSERT(spOrthoCamera, "Ortho Graphic Camera is null in Scene Hierarchy Panel");

				float fOrthoSize = spOrthoCamera->GetOrthoSize();
				if (ImGui::DragFloat("Size", &fOrthoSize))
				{
					spOrthoCamera->SetOrthoSize(fOrthoSize);
				}

				float fOrthoNear = spOrthoCamera->GetNearClip();
				if (ImGui::DragFloat("Near", &fOrthoNear))
				{
					spOrthoCamera->SetNearClip(fOrthoNear);
				}

				float fOrthoFar = spOrthoCamera->GetFarClip();
				if (ImGui::DragFloat("Far", &fOrthoFar))
				{
					spOrthoCamera->SetFarClip(fOrthoFar);
				}
				break;
			}

			default:
				break;
			}
		});


	DrawComponent<SpriteRenderComponent>("Sprite Render", spEntity, [](auto& component)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
			ImGui::Button("Texture", ImVec2(100.f, 0.0f));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					std::filesystem::path filePath((const wchar_t*)payload->Data);
					if (filePath.extension().string() == ".png")
					{
						auto texturePath = sAssetsDirector / filePath;
						component.spTexture = Texture2D::Create(texturePath.string());
					}
					else
					{
						LOG_DEV_WARN("Could not load {0} - not a texture file", filePath.filename().string());
					}
					
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::DragFloat("Tiling Facto", &component.TilingFactor, 0.1f, 0.f, 10.f);
		});

	DrawComponent<RigidBody2DComponent>("Rigidbody 2D", spEntity, [](auto& component)
		{
			auto sCurrentBodyType = BodyTypeStrings[static_cast<unsigned int>(component.Type)];

			if (ImGui::BeginCombo("Body Type", sCurrentBodyType.c_str()))
			{
				for (size_t i = 0; i < BodyTypeStrings.size(); i++)
				{
					bool bSelected = sCurrentBodyType == BodyTypeStrings[i];
					if (ImGui::Selectable(BodyTypeStrings[i].c_str(), bSelected))
					{
						component.Type = static_cast<BodyType>(i);
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();

			}

			ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
		});

	DrawComponent<BoxCollider2DComponent>("BoxCollider 2D", spEntity, [](auto& component)
		{
			ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
			ImGui::DragFloat2("Size", glm::value_ptr(component.Size));
			ImGui::DragFloat("Density", &component.Density, 0.01f, 0.f, 1.f);
			ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.f, 1.f);
			ImGui::DragFloat("Restitution",&component.RestitutionThreshold, 0.01f, 0.f, 1.f);
			ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.f);
		});

}

SAND_TABLE_NAMESPACE_END


