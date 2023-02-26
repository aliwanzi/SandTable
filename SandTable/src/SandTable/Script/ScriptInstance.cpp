#include "pch.h"
#include "ScriptInstance.h"
#include "SandTable/Scene/UUID.h"

SAND_TABLE_NAMESPACE_BEGIN

ScriptInstance::ScriptInstance(Ref<ScriptClass>spEntityClass, Ref<ScriptClass> spScriptClass, UUID entityID) :
	m_spScriptClass(spScriptClass)
{
	m_pMonoMethodConStruct = spEntityClass->GetMonoMethod(".ctor", 1);
	m_pMonoMethodOnCreate = m_spScriptClass->GetMonoMethod("OnCreate", 0);
	m_pMonoMethodOnUpdate = m_spScriptClass->GetMonoMethod("OnUpdate", 1);

	void* pParam = &entityID;
	m_spScriptClass->InvokeMethod(m_pMonoMethodConStruct, &pParam);
}

Ref<ScriptClass> ScriptInstance::GetScriptClass()
{
	return m_spScriptClass;
}

void ScriptInstance::InvokeOnCreate()
{
	if (m_pMonoMethodOnCreate != nullptr)
	{
		m_spScriptClass->InvokeMethod(m_pMonoMethodOnCreate);
	}
}

void ScriptInstance::InVokeOnUpdate(TimeStep fTimeStep)
{
	if (m_pMonoMethodOnUpdate!=nullptr)
	{
		void* pParam = &fTimeStep;
		m_spScriptClass->InvokeMethod(m_pMonoMethodOnUpdate, &pParam);
	}
}

SAND_TABLE_NAMESPACE_END
