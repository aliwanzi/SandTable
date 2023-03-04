#include "pch.h"
#include "ScriptEntityClass.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"

SAND_TABLE_NAMESPACE_BEGIN

ScriptEntityClass::ScriptEntityClass(const std::string& sClassNameSpace, const std::string& sClassName,
	const Ref<MonoImage>& spMonoImage, const Ref<MonoDomain>& spMonoDomain)
{
	m_pMonoClass = mono_class_from_name(spMonoImage.get(), sClassNameSpace.c_str(), sClassName.c_str());
	m_pMonoObject = mono_object_new(spMonoDomain.get(), m_pMonoClass);
	mono_runtime_object_init(m_pMonoObject);
}

MonoObject* ScriptEntityClass::GetMonoObject()
{
	return m_pMonoObject;
}

MonoMethod* ScriptEntityClass::GetMonoMethod(const std::string& sMethodName, unsigned int uiParaCount)
{
	return mono_class_get_method_from_name(m_pMonoClass, sMethodName.c_str(), uiParaCount);
}

void ScriptEntityClass::InvokeMethod(MonoMethod* pMonoMethod, void** pParams)
{
	mono_runtime_invoke(pMonoMethod, m_pMonoObject, pParams, nullptr);
}

void ScriptEntityClass::AddScriptEntityField(const std::string& sFieldName, const ScriptField& scriptField)
{
	m_mapScriptEntityField[sFieldName] = scriptField;
}

const MapScriptField& ScriptEntityClass::GetScriptEntityFields() const
{
	return m_mapScriptEntityField;
}

void ScriptEntityClass::SetFieldValueBuffer(const std::string& sFieldName, void* buffer)
{
	auto iter = m_mapScriptEntityField.find(sFieldName);
	if (iter != m_mapScriptEntityField.end())
	{
		iter->second.FieldValue->SetBuffer(buffer);
		mono_field_set_value(m_pMonoObject, iter->second.FieldMonoClass, iter->second.FieldValue->GetBuffer());
	}
}

void ScriptEntityClass::GetFieldValueInternal(MonoClassField* field, void* buffer)
{
	mono_field_get_value(m_pMonoObject, field, buffer);
}

void ScriptEntityClass::SetFieldValueInternal(MonoClassField* field, void* buffer)
{
	mono_field_set_value(m_pMonoObject, field, buffer);
}

SAND_TABLE_NAMESPACE_END
