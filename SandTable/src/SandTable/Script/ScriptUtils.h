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

inline std::string ScriptFieldTypeToString(const ScriptFieldType& eFieldType)
{
	switch (eFieldType)
	{
	case ScriptFieldType::None:    return "None";
	case ScriptFieldType::Float:   return "Float";
	case ScriptFieldType::Double:  return "Double";
	case ScriptFieldType::Bool:    return "Bool";
	case ScriptFieldType::Char:    return "Char";
	case ScriptFieldType::Byte:    return "Byte";
	case ScriptFieldType::Short:   return "Short";
	case ScriptFieldType::Int:     return "Int";
	case ScriptFieldType::Long:    return "Long";
	case ScriptFieldType::UByte:   return "UByte";
	case ScriptFieldType::UShort:  return "UShort";
	case ScriptFieldType::UInt:    return "UInt";
	case ScriptFieldType::ULong:   return "ULong";
	case ScriptFieldType::Vector2: return "Vector2";
	case ScriptFieldType::Vector3: return "Vector3";
	case ScriptFieldType::Vector4: return "Vector4";
	case ScriptFieldType::Entity:  return "Entity";
	}
	SAND_TABLE_ASSERT(false, "Unknown ScriptFieldType");
	return "None";
}

inline ScriptFieldType ScriptFieldTypeFromString(const std::string_view& sFieldType)
{
	if (sFieldType == "None")    return ScriptFieldType::None;
	if (sFieldType == "Float")   return ScriptFieldType::Float;
	if (sFieldType == "Double")  return ScriptFieldType::Double;
	if (sFieldType == "Bool")    return ScriptFieldType::Bool;
	if (sFieldType == "Char")    return ScriptFieldType::Char;
	if (sFieldType == "Byte")    return ScriptFieldType::Byte;
	if (sFieldType == "Short")   return ScriptFieldType::Short;
	if (sFieldType == "Int")     return ScriptFieldType::Int;
	if (sFieldType == "Long")    return ScriptFieldType::Long;
	if (sFieldType == "UByte")   return ScriptFieldType::UByte;
	if (sFieldType == "UShort")  return ScriptFieldType::UShort;
	if (sFieldType == "UInt")    return ScriptFieldType::UInt;
	if (sFieldType == "ULong")   return ScriptFieldType::ULong;
	if (sFieldType == "Vector2") return ScriptFieldType::Vector2;
	if (sFieldType == "Vector3") return ScriptFieldType::Vector3;
	if (sFieldType == "Vector4") return ScriptFieldType::Vector4;
	if (sFieldType == "Entity")  return ScriptFieldType::Entity;

	SAND_TABLE_ASSERT(false, "Unknown ScriptFieldType");
	return ScriptFieldType::None;
}

SAND_TABLE_NAMESPACE_END