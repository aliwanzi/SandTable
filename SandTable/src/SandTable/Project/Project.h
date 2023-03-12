#pragma once

SAND_TABLE_NAMESPACE_BEGIN

struct ProjectConfig
{
	std::string Name = "Untitled";

	std::filesystem::path StartScene;

	std::filesystem::path AssetDirectory;
	std::filesystem::path ScriptModulePath;
};

class Project
{
public:
	Ref<ProjectConfig>& GetProjectConfig();

	const std::filesystem::path& GetProjectDirectory();
	std::filesystem::path GetAssetDirectory();
	std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& sFilePath);

	static Ref<Project>& GetProjectInstance();
	static bool Load(const std::filesystem::path& sFilePath);
	static bool SaveActive(const std::filesystem::path& sFilePath);

private:
	Project();
	Project(Project&) = delete;
	Project& operator=(const Project&) = delete;

private:
	static Ref<Project> m_spActiveProject;

	Ref<ProjectConfig> m_spProjectConfig;
	std::filesystem::path m_sActiveProjectDirectory;
};

SAND_TABLE_NAMESPACE_END