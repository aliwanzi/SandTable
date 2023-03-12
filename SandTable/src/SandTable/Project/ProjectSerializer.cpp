#include "pch.h"
#include "ProjectSerializer.h"
#include <yaml-cpp/yaml.h>

SAND_TABLE_NAMESPACE_BEGIN
ProjectSerializer::ProjectSerializer(Ref<Project> spProject):
	m_spProject(spProject)
{
}

bool ProjectSerializer::Serialize(const std::filesystem::path& sFilePath)
{
	const auto& spProjectConfig = m_spProject->GetProjectConfig();
	YAML::Emitter out;
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Project" << YAML::Value;
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Name" << YAML::Value << spProjectConfig->Name;
			out << YAML::Key << "StartScene" << YAML::Value << spProjectConfig->StartScene.string();
			out << YAML::Key << "AssetDirectory" << YAML::Value << spProjectConfig->AssetDirectory.string();
			out << YAML::Key << "ScriptModulePath" << YAML::Value << spProjectConfig->ScriptModulePath.string();
			out << YAML::EndMap;
		}
		out << YAML::EndMap;
	}
	std::ofstream stream(sFilePath);
	stream << out.c_str();
	return false;
}

bool ProjectSerializer::DeSerialize(const std::filesystem::path& sFilePath)
{
	auto& spProjectConfig = m_spProject->GetProjectConfig();

	YAML::Node data;
	try
	{
		data = YAML::LoadFile(sFilePath.string());
	}
	catch (YAML::ParserException e)
	{
		LOG_DEV_ERROR("Failed to load file '{0}'\n    {1}", sFilePath, e.what());
		return false;
	}

	auto projectNode = data["Project"];
	if (!projectNode)
	{
		return false;
	}

	spProjectConfig->Name = projectNode["Name"].as<std::string>();
	spProjectConfig->StartScene = projectNode["StartScene"].as<std::string>();
	spProjectConfig->AssetDirectory = projectNode["AssetDirectory"].as<std::string>();
	spProjectConfig->ScriptModulePath = projectNode["ScriptModulePath"].as<std::string>();

	return true;
}

SAND_TABLE_NAMESPACE_END
