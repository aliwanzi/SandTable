#pragma once
#include "ScriptField.h"

SAND_TABLE_NAMESPACE_BEGIN

class ScriptEntityClass
{
public:
	ScriptEntityClass();
	ScriptEntityClass(const std::string& sClassNameSpace, const std::string& sClassName,
		const Ref<MonoImage>& spMonoImage, const Ref<MonoDomain>& spMonoDomain);

	MonoMethod* GetMonoMethod(const std::string& sMethodName, unsigned int uiParaCount);
	void InvokeMethod(MonoMethod* pMonoMethod, void** params = nullptr);

	void AddScriptEntityField(const std::string& sFieldName, const ScriptField& scriptField);
	const MapScriptField& GetScriptEntityFields() const;

	template<typename T>
	T GetFieldValue(const std::string& sFieldName)
	{
		SAND_TABLE_ASSERT(sizeof(T) <= 16, "Type is too large!");
		auto iter = m_mapScriptEntityField.find(sFieldName);
		if (iter != m_mapScriptEntityField.end())
		{
			GetFieldValueInternal(iter->second.FieldMonoClass, iter->second.FieldValue->GetBuffer());
			return iter->second.FieldValue->GetValue<T>();
		}
		return T();
	}

	template<typename T>
	void SetFieldValue(const std::string& sFieldName, const T& value)
	{
		SAND_TABLE_ASSERT(sizeof(T) <= 16, "Type is too large!");
		auto iter = m_mapScriptEntityField.find(sFieldName);
		if (iter != m_mapScriptEntityField.end())
		{
			iter->second.FieldValue->SetValue<T>(value);
			SetFieldValueInternal(iter->second.FieldMonoClass, iter->second.FieldValue->GetBuffer());
		}
	}

	void SetFieldValueBuffer(const std::string& sFieldName, void* buffer);

private:

	void GetFieldValueInternal(MonoClassField* field, void* buffer);
	void SetFieldValueInternal(MonoClassField* field, void* buffer);

private:
	MonoClass* m_pMonoClass;
	MonoObject* m_pMonoObject;
	MapScriptField m_mapScriptEntityField;
};

using MapScriptEntityClass = std::unordered_map<std::string, Ref<ScriptEntityClass>>;
SAND_TABLE_NAMESPACE_END

