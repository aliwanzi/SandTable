#pragma once
extern "C"
{
	typedef struct	_MonoClass		MonoClass;
	typedef struct	_MonoObject		MonoObject;
	typedef struct	_MonoMethod		MonoMethod;
	typedef struct	_MonoAssembly	MonoAssembly;
	typedef struct	_MonoDomain		MonoDomain;
	typedef struct	_MonoImage		MonoImage;
	typedef struct	_MonoClassField	MonoClassField;
}

SAND_TABLE_NAMESPACE_BEGIN

enum class ScriptFieldType
{
	None = 0,
	Float, Double,
	Bool, Char, Byte, Short, Int, Long,
	UByte, UShort, UInt, ULong,
	Vector2, Vector3, Vector4,
	Entity
};

struct ScriptField
{
	ScriptFieldType Type;
	MonoClassField* ClassField;
};

const std::unordered_map<std::string, ScriptFieldType> ScriptFieldTypeMap
{
	{ "System.Single",		ScriptFieldType::Float },
	{ "System.Double",		ScriptFieldType::Double },
	{ "System.Boolean",		ScriptFieldType::Bool },
	{ "System.Char",		ScriptFieldType::Char },
	{ "System.Int16",		ScriptFieldType::Short },
	{ "System.Int32",		ScriptFieldType::Int },
	{ "System.Int64",		ScriptFieldType::Long },
	{ "System.Byte",		ScriptFieldType::Byte },
	{ "System.UInt16",		ScriptFieldType::UShort },
	{ "System.UInt32",		ScriptFieldType::UInt },
	{ "System.UInt64",		ScriptFieldType::ULong },

	{ "SandTable.Vector2",	ScriptFieldType::Vector2 },
	{ "SandTable.Vector3",	ScriptFieldType::Vector3 },
	{ "SandTable.Vector4",	ScriptFieldType::Vector4 },

	{ "SandTable.Entity",	ScriptFieldType::Entity },
};

class ScriptClass
{
public:
	ScriptClass() = default;
	ScriptClass(const std::string& sClassNameSpace, const std::string& sClassName,
		const Ref<MonoDomain>& spMonoDomain, const Ref<MonoImage>& spMonoImage);

	MonoMethod* GetMonoMethod(const std::string& sMethodName, unsigned int uiParaCount);
	void InvokeMethod(MonoMethod* pMonoMethod, void** params = nullptr);

	void AddScriptField(const std::string& sFieldName, const ScriptField& scriptField);
	const std::unordered_map<std::string, ScriptField>& GetScriptFields() const;

	template<typename T>
	T GetFieldValue(const std::string& sFieldName)
	{
		SAND_TABLE_ASSERT(sizeof(T) <= 16, "Type is too large!");
		if (GetFieldValueInternal(sFieldName, m_pFieldValueBuffer))
		{
			return *(T*)m_pFieldValueBuffer;
		}
		return T();
	}

	template<typename T>
	bool SetFieldValue(const std::string& sFieldName, const T& value)
	{
		return SetFieldValueInternal(sFieldName, &value);
	}

private:
	bool GetFieldValueInternal(const std::string& sFieldName, void* buffer);
	bool SetFieldValueInternal(const std::string& sFieldName, const void* value);

private:
	MonoClass* m_pMonoClass;
	MonoObject* m_pMonoObject;
	std::unordered_map<std::string, ScriptField> m_mapScriptField;
	inline static char m_pFieldValueBuffer[16];
};
SAND_TABLE_NAMESPACE_END

