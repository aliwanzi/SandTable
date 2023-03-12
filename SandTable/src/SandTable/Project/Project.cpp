#include "pch.h"
#include "Project.h"
#include "ProjectSerializer.h"

SAND_TABLE_NAMESPACE_BEGIN
Ref<Project> Project::m_spActiveProject = nullptr;

Project::Project():
	m_spProjectConfig(CreateRef<ProjectConfig>()),
	m_sActiveProjectDirectory("")
{

}

Ref<ProjectConfig>& Project::GetProjectConfig()
{
	return m_spProjectConfig;
}

const std::filesystem::path& Project::GetProjectDirectory()
{
	return m_sActiveProjectDirectory;
}

std::filesystem::path Project::GetAssetDirectory()
{
	return GetProjectDirectory() / m_spProjectConfig->AssetDirectory;
}

std::filesystem::path Project::GetAssetFileSystemPath(const std::filesystem::path& sFilePath)
{
	return GetAssetDirectory() / sFilePath;
}

Ref<Project>& Project::GetProjectInstance()
{
	if (m_spActiveProject == nullptr)
	{
		m_spActiveProject = Ref<Project>(new Project());
	}
	return m_spActiveProject;
}

bool Project::Load(const std::filesystem::path& sFilePath)
{
	auto spProject = Ref<Project>(new Project());
	auto spProjectSerializer = CreateRef<ProjectSerializer>(spProject);
	if (spProjectSerializer->DeSerialize(sFilePath))
	{
		m_spActiveProject = spProject;
		m_spActiveProject->m_sActiveProjectDirectory = sFilePath.parent_path();
		return true;
	}
	return false;
}

bool Project::SaveActive(const std::filesystem::path& sFilePath)
{
	auto spProjectSerializer = CreateRef<ProjectSerializer>(m_spActiveProject);
	if (spProjectSerializer->Serialize(sFilePath))
	{
		m_spActiveProject->m_sActiveProjectDirectory = sFilePath.parent_path();
		return true;
	}
	return false;
}


SAND_TABLE_NAMESPACE_END