#include "pch.h"
#include "SandTable/ImGui/ContentBrowserPanel.h"
#include "SandTable/Render/Texture/Texture2D.h"
#include "SandTable/Project/Project.h"
#include <imgui.h>
SAND_TABLE_NAMESPACE_BEGIN

namespace
{
	const float fPadding = 8.0f;
	const float fThumbnailSize = 64.0f;
	const float fCellSize = fPadding + fThumbnailSize;
}

ContentBrowserPanel::ContentBrowserPanel()
{
	m_sBaseDirectory = Project::GetProjectInstance()->GetAssetDirectory();
	m_sCurrentDirectory = m_sBaseDirectory;
	m_spDirectoryIcon = Texture2D::Create("resources/Icons/ContentBrowser/DirectoryIcon.png");
	m_spFileIcon = Texture2D::Create("resources/Icons/ContentBrowser/FileIcon.png");
}

void ContentBrowserPanel::OnImGuiRender()
{
	ImGui::Begin("Context Browser");
	if (m_sCurrentDirectory != std::filesystem::path(m_sBaseDirectory) &&
		ImGui::Button("<-"))
	{
		m_sCurrentDirectory = m_sCurrentDirectory.parent_path();
	}

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int iColumnCount = static_cast<int>(panelWidth / fCellSize);
	if (iColumnCount < 1)
		iColumnCount = 1;

	ImGui::Columns(iColumnCount, 0, false);

	for (auto& directoryEntry : std::filesystem::directory_iterator(m_sCurrentDirectory))
	{
		const auto& path = directoryEntry.path();
		std::string filenameString = path.filename().string();

		ImGui::PushID(filenameString.c_str());
		auto spIcon = directoryEntry.is_directory() ? m_spDirectoryIcon : m_spFileIcon;
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::ImageButton((ImTextureID)spIcon->GetRenderID(),
			ImVec2(fThumbnailSize, fThumbnailSize), ImVec2(0, 1), ImVec2(1, 0));

		if (ImGui::BeginDragDropSource())
		{
			const wchar_t* itemPath = path.c_str();
			ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
			ImGui::EndDragDropSource();
		}

		ImGui::PopStyleColor();
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (directoryEntry.is_directory())
				m_sCurrentDirectory /= path.filename();
		}
		ImGui::TextWrapped(filenameString.c_str());

		ImGui::NextColumn();

		ImGui::PopID();
	}
	ImGui::End();
}

SAND_TABLE_NAMESPACE_END
