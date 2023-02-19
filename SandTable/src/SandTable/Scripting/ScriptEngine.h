#pragma once

SAND_TABLE_NAMESPACE_BEGIN
class ScriptClass;
class Scene;
class Entity;
class ScriptEngine
{
public:
	static void Init();
	static void LoadAssembly(const std::filesystem::path& sAssemblyPath);

	static bool EntityClassExit(const std::string& sFullClassName);
	static const std::unordered_map<std::string, Ref<ScriptClass>>& GetEntityClass();
	static void OnCreateEntity(Ref<Entity> spEntity);
	static void OnUpdateEntity(Ref<Entity> spEntity,TimeStep fTimeStep);

	static void OnRuntimeStart(Ref<Scene> spScene);
	static void OnRuntimeStop();
private:
	static void InitMono();
	static void InvokeClass(const std::string& sClassNameSpace, const std::string& sClassName);
	static void LoadAssemblyClass();
};

SAND_TABLE_NAMESPACE_END

