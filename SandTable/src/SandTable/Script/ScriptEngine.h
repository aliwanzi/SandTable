#pragma once
#include "ScriptClass.h"

SAND_TABLE_NAMESPACE_BEGIN
class Scene;
class Entity;
class ScriptInstance;
class UUID;

class ScriptEngine
{
public:
	static void Init();
	static void LoadCoreAssembly(const std::filesystem::path& sAssemblyPath);
	static void LoadAppAssembly(const std::filesystem::path& sAssemblyPath);

	static const Ref<MonoImage>& GetCoreAssemblyImage();

	static bool EntityClassExit(const std::string& sFullClassName);
	static void OnCreateEntity(Ref<Entity> spEntity);
	static void OnUpdateEntity(Ref<Entity> spEntity,TimeStep fTimeStep);

	static const std::unordered_map<std::string, Ref<ScriptClass>>& GetEntityClass();
	static Ref<ScriptInstance> GetEntityScriptInstance(UUID uiEntityID);

	static void OnRuntimeStart(Ref<Scene> spScene);
	static const Ref<Scene>& GetRuntimeScene();
	static void OnRuntimeStop();
private:
	static void InitMono();
	static void LoadAssemblyClass();
};

SAND_TABLE_NAMESPACE_END

