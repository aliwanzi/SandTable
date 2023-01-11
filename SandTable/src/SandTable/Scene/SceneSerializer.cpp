#include "pch.h"
#include "SceneSerializer.h"
#include "SandTable/Scene/Entity.h"
#include "SandTable/Scene/Components.h"
#include <yaml-cpp/yaml.h>

SAND_TABLE_NAMESPACE_BEGIN
namespace 
{
	void SerializeEntity(YAML::Emitter& out, Entity& entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "12837";
		out << YAML::EndMap;
		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;
			out << YAML::EndMap;
		}
		out << YAML::EndMap;
	}
}

SceneSerializer::SceneSerializer(const Ref<Scene>& spScene):m_spScene(spScene)
{

}

void SceneSerializer::Serialize(const std::string& sFilePath)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << "Untitled";
	out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
	auto spRegistry = m_spScene->Registry();
	spRegistry->each([&](auto entityID)
	{
			Entity entity(spRegistry, entityID);
			if (!entity)
			{
				return;
			}
			SerializeEntity(out, entity);
	});
	out << YAML::EndSeq;
	out << YAML::EndMap;

	std::ofstream fout(sFilePath);
	fout << out.c_str();

}

void SceneSerializer::SerializeRuntime(const std::string& sFilePath)
{

}

void SceneSerializer::DeSerialize(const std::string& sFilePath)
{

}

void SceneSerializer::DeSerializeRuntime(const std::string& sFilePath)
{

}

SAND_TABLE_NAMESPACE_END