#pragma once
#include "SandTable/Scene/Scene.h"

SAND_TABLE_NAMESPACE_BEGIN

class SceneSerializer
{
public:
	SceneSerializer(const Ref<Scene>& spScene);

	void Serialize(const std::string& sFilePath);
	void SerializeRuntime(const std::string& sFilePath);
	bool DeSerialize(const std::string& sFilePath);
	bool DeSerializeRuntime(const std::string& sFilePath);
	void SetSelectedScene(const Ref<Scene>& spScene);
private:
	Ref<Scene> m_spScene;
};

SAND_TABLE_NAMESPACE_END
