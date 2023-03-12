#pragma once
#include "Project.h"
SAND_TABLE_NAMESPACE_BEGIN
class ProjectSerializer
{
public:
	ProjectSerializer(Ref<Project> spProject);

	bool Serialize(const std::filesystem::path& sFilePath);
	bool DeSerialize(const std::filesystem::path& sFilePath);
private:
	Ref<Project> m_spProject;
};

SAND_TABLE_NAMESPACE_END
