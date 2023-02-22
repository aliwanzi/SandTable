#pragma once
#include "ScriptClass.h"

SAND_TABLE_NAMESPACE_BEGIN
class UUID;
class ScriptInstance
{
public:
	ScriptInstance(Ref<ScriptClass> spScriptEntityClass, Ref<ScriptClass> spScriptClass, UUID entityID);
	void InvokeOnCreate();
	void InVokeOnUpdate(TimeStep fTimeStep);
private:
	Ref<ScriptClass> m_spScriptClass;
	MonoMethod* m_pMonoMethodConStruct;
	MonoMethod* m_pMonoMethodOnCreate;
	MonoMethod* m_pMonoMethodOnUpdate;
};

SAND_TABLE_NAMESPACE_END

