#include "pch.h"
#include "ScriptGlue.h"
#include "mono/metadata/object.h"

SAND_TABLE_NAMESPACE_BEGIN

namespace
{
	void CppFunc()
	{
		std::cout << "This is written in C++!" << std::endl;
	}

	void NativeLogInt(MonoString* pStrLog, int iPar)
	{
		char* pLog = mono_string_to_utf8(pStrLog);
		std::cout << pLog << " " << iPar << std::endl;
		mono_free(pLog);
	}

	void NativeLogVec3(glm::vec3* vec3Par, glm::vec3* vec3Out)
	{
		LOG_DEV_INFO("pStrLog Value:{0}", *vec3Par);
		*vec3Out = glm::normalize(*vec3Par);
	}

	float NativeLogVec3Dot(glm::vec3* vec3Par)
	{
		LOG_DEV_INFO("pStrLog Value:{0}", *vec3Par);
		return glm::dot(*vec3Par, *vec3Par);
	}

#define SAND_TABLE_INTERNAL_CALL(Name) mono_add_internal_call("SandTable.Main::" #Name , Name)
}

void ScriptGlue::RegisterFunctions()
{
	SAND_TABLE_INTERNAL_CALL(CppFunc);
	SAND_TABLE_INTERNAL_CALL(NativeLogInt);
	SAND_TABLE_INTERNAL_CALL(NativeLogVec3);
	SAND_TABLE_INTERNAL_CALL(NativeLogVec3Dot);
}

SAND_TABLE_NAMESPACE_END
