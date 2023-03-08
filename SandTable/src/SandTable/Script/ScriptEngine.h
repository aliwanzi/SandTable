#pragma once
#include "ScriptEntityClass.h"

SAND_TABLE_NAMESPACE_BEGIN
class Scene;
class Entity;
class UUID;
class ScriptEntityInstance;

class ScriptEngine
{
public:
	static void Init();

	static MonoImage* GetCoreAssemblyImage();

	static void OnCreateEntity(Ref<Entity> spEntity);
	static void OnUpdateEntity(Ref<Entity> spEntity,TimeStep fTimeStep);

	static const MapScriptEntityClass& GetScriptEntityClassMap();
	static bool ScriptEntityClassExit(const std::string& sEntityName);
	static Ref<ScriptEntityClass> GetScriptEntityClass(const std::string& sEntityName);
	static Ref<ScriptEntityInstance> GetScriptEntityInstance(const UUID& uiEntityID);

	static MapScriptField& GetScriptFieldMap(const UUID& uiEntityID);

	static MonoObject* GetMonoObject(const UUID& uiEntityID);

	static void OnRuntimeStart(Ref<Scene> spScene);
	static const Ref<Scene>& GetRuntimeScene();
	static void OnRuntimeStop();

	static void ReloadAssembly();
private:
	static void InitMono();
	static void LoadAssemblyAndMonoImage();
	static void LoadAssemblyClass();
};

SAND_TABLE_NAMESPACE_END

