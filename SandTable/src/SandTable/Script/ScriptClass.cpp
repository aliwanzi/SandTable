#include "pch.h"
#include "ScriptClass.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"

SAND_TABLE_NAMESPACE_BEGIN

ScriptClass::ScriptClass(const std::string& sClassNameSpace, const std::string& sClassName, 
	const Ref<MonoDomain>& spMonoDomain,const Ref<MonoImage>& spMonoImage)
{
	m_pMonoClass = mono_class_from_name(spMonoImage.get(), sClassNameSpace.c_str(), sClassName.c_str());
	m_pMonoObject = mono_object_new(spMonoDomain.get(), m_pMonoClass);
	mono_runtime_object_init(m_pMonoObject);
}

MonoMethod* ScriptClass::GetMonoMethod(const std::string& sMethodName, unsigned int uiParaCount)
{
	return mono_class_get_method_from_name(m_pMonoClass, sMethodName.c_str(), uiParaCount);
}

void ScriptClass::InvokeMethod(MonoMethod* pMonoMethod, void** pParams)
{
	mono_runtime_invoke(pMonoMethod, m_pMonoObject, pParams, nullptr);
}

void ScriptClass::AddScriptField(const std::string& sFieldName, const ScriptField& scriptField)
{
	m_mapScriptField[sFieldName] = scriptField;
}

const std::unordered_map<std::string, ScriptField>& ScriptClass::GetScriptFields() const
{
	return m_mapScriptField;
}

bool ScriptClass::GetFieldValueInternal(const std::string& sFieldName, void* buffer)
{
	auto iter = m_mapScriptField.find(sFieldName);
	if (iter != m_mapScriptField.end())
	{
		mono_field_get_value(m_pMonoObject, iter->second.ClassField, buffer);
		return true;
	}
	return false;
}

bool ScriptClass::SetFieldValueInternal(const std::string& sFieldName, const void* value)
{
	auto iter = m_mapScriptField.find(sFieldName);
	if (iter != m_mapScriptField.end())
	{
		mono_field_set_value(m_pMonoObject, iter->second.ClassField, (void*)value);
		return true;
	}
	return false;
}

SAND_TABLE_NAMESPACE_END
