#pragma once
#include "ScriptUtils.h"

SAND_TABLE_NAMESPACE_BEGIN

class ScriptFieldValue
{
public:
	ScriptFieldValue()
	{
		memset(m_pFieldValueBuffer, 0, sizeof(m_pFieldValueBuffer));
	}

	void* GetBuffer()
	{
		return m_pFieldValueBuffer;
	}

	template<typename T>
	T GetValue() const
	{
		return *(T*)m_pFieldValueBuffer;
	}

	template<typename T>
	void SetValue(const T& value)
	{
		SAND_TABLE_ASSERT(sizeof(T) <= 16, "Type too large!");
		memcpy(m_pFieldValueBuffer, &value, sizeof(T));
	}

	void SetBuffer(const void* buffer)
	{
		memcpy(m_pFieldValueBuffer, buffer, 16);
	}

private:
	char m_pFieldValueBuffer[16];
};

struct ScriptField
{
	ScriptFieldType			FieldType;
	Ref<ScriptFieldValue>	FieldValue;
	MonoClassField*			FieldMonoClass;
};

using MapScriptField = std::unordered_map<std::string, ScriptField>;

SAND_TABLE_NAMESPACE_END
