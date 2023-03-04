#include "pch.h"
#include "ScriptEntityInstance.h"
#include "ScriptEntityClass.h"
#include "SandTable/Scene/UUID.h"

SAND_TABLE_NAMESPACE_BEGIN

ScriptEntityInstance::ScriptEntityInstance(Ref<ScriptEntityClass> spCoreEntityClass, Ref<ScriptEntityClass> spAppEntityClass, UUID entityID) :
	m_spAppEntityClass(spAppEntityClass)
{
	m_pMonoMethodConStruct = spCoreEntityClass->GetMonoMethod(".ctor", 1);
	m_pMonoMethodOnCreate = m_spAppEntityClass->GetMonoMethod("OnCreate", 0);
	m_pMonoMethodOnUpdate = m_spAppEntityClass->GetMonoMethod("OnUpdate", 1);

	void* pParam = &entityID;
	m_spAppEntityClass->InvokeMethod(m_pMonoMethodConStruct, &pParam);
}

Ref<ScriptEntityClass> ScriptEntityInstance::GetAppScriptEntityClass()
{
	return m_spAppEntityClass;
}

void ScriptEntityInstance::InvokeOnCreate()
{
	if (m_pMonoMethodOnCreate != nullptr)
	{
		m_spAppEntityClass->InvokeMethod(m_pMonoMethodOnCreate);
	}
}

void ScriptEntityInstance::InVokeOnUpdate(TimeStep fTimeStep)
{
	if (m_pMonoMethodOnUpdate!=nullptr)
	{
		void* pParam = &fTimeStep;
		m_spAppEntityClass->InvokeMethod(m_pMonoMethodOnUpdate, &pParam);
	}
}

MonoObject* ScriptEntityInstance::GetMonoObject()
{
	return m_spAppEntityClass->GetMonoObject();
}

SAND_TABLE_NAMESPACE_END
