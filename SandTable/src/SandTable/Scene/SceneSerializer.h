#pragma once
#include "SandTable/Scene/Scene.h"

SAND_TABLE_NAMESPACE_BEGIN

class SceneSerializer
{
public:
	SceneSerializer(const Ref<Scene>& spScene);

	void Serialize(const std::string& sFilePath);
	void SerializeRuntime(const std::string& sFilePath);
	void DeSerialize(const std::string& sFilePath);
	void DeSerializeRuntime(const std::string& sFilePath);

private:
	Ref<Scene> m_spScene;
};

SAND_TABLE_NAMESPACE_END
