#include "pch.h"
#include "ScriptInstance.h"

SAND_TABLE_NAMESPACE_BEGIN

ScriptInstance::ScriptInstance(Ref<ScriptClass> spScriptClass):
	m_spScriptClass(spScriptClass)
{
	m_pMonoMethodOnCreate = m_spScriptClass->GetMonoMethod("OnCreate", 0);
	m_pMonoMethodOnUpdate = m_spScriptClass->GetMonoMethod("OnUpdate", 1);
}

void ScriptInstance::InvokeOnCreate()
{
	m_spScriptClass->InvokeMethod(m_pMonoMethodOnCreate);
}

void ScriptInstance::InVokeOnUpdate(TimeStep fTimeStep)
{
	void* pParam = &fTimeStep;
	m_spScriptClass->InvokeMethod(m_pMonoMethodOnUpdate, &pParam);
}

SAND_TABLE_NAMESPACE_END
