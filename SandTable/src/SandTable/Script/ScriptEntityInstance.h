#pragma once
#include "ScriptUtils.h"

SAND_TABLE_NAMESPACE_BEGIN

class UUID;
class ScriptEntityClass;
class ScriptEntityInstance
{
public:
	ScriptEntityInstance(Ref<ScriptEntityClass> spScriptEntityClass, Ref<ScriptEntityClass> spScriptClass, UUID entityID);
	Ref<ScriptEntityClass> GetAppScriptEntityClass();
	void InvokeOnCreate();
	void InVokeOnUpdate(TimeStep fTimeStep);
	MonoObject* GetMonoObject();
private:
	Ref<ScriptEntityClass> m_spAppEntityClass;
	MonoMethod* m_pMonoMethodConStruct;
	MonoMethod* m_pMonoMethodOnCreate;
	MonoMethod* m_pMonoMethodOnUpdate;
};

using MapScriptEntityInstance = std::unordered_map<UUID, Ref<ScriptEntityInstance>>;

SAND_TABLE_NAMESPACE_END

