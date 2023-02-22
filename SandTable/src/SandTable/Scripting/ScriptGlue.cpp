#include "pch.h"
#include "ScriptGlue.h"
#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"
#include "SandTable/Scene/UUID.h"
#include "SandTable/Scripting/ScriptEngine.h"
#include "SandTable/Scene/Scene.h"
#include "SandTable/Core/KeyCode.h"
#include "SandTable/Core/Input.h"

SAND_TABLE_NAMESPACE_BEGIN

namespace
{
	void NativeLogInt(MonoString* pStrLog, int iPar)
	{
		char* pLog = mono_string_to_utf8(pStrLog);
		std::cout << pLog << " " << iPar << std::endl;
		mono_free(pLog);
	}

	void NativeLogVec3Out(glm::vec3* vec3Par, glm::vec3* vec3Out)
	{
		LOG_DEV_INFO("pStrLog Value:{0}", *vec3Par);
		*vec3Out = glm::normalize(*vec3Par);
	}

	float NativeLogVec3Dot(glm::vec3* vec3Par)
	{
		LOG_DEV_INFO("pStrLog Value:{0}", *vec3Par);
		return glm::dot(*vec3Par, *vec3Par);
	}

	void EntityGetTranslation(UUID entityID, glm::vec3* vec3Par)
	{
		auto spScene = ScriptEngine::GetRuntimeScene();
		auto spEntity = spScene->GetEntityByUUID(entityID);
		*vec3Par = spEntity->GetComponent<TransformComponent>().Translation;
	}

	void EntitySetTranslation(UUID entityID, glm::vec3* vec3Par)
	{
		auto spScene = ScriptEngine::GetRuntimeScene();
		auto spEntity = spScene->GetEntityByUUID(entityID);
		spEntity->GetComponent<TransformComponent>().Translation = *vec3Par;
	}

	bool InputIsKeyDown(KeyCode keyCode)
	{
		return Input::IsKeyPressed(keyCode);
	}

#define SAND_TABLE_INTERNAL_CALL(Name) mono_add_internal_call("SandTable.InternalCall::" #Name, Name)
}

void ScriptGlue::RegisterFunctions()
{  
	SAND_TABLE_INTERNAL_CALL(NativeLogInt);
	SAND_TABLE_INTERNAL_CALL(NativeLogVec3Out);
	SAND_TABLE_INTERNAL_CALL(NativeLogVec3Dot);

	SAND_TABLE_INTERNAL_CALL(EntityGetTranslation);
	SAND_TABLE_INTERNAL_CALL(EntitySetTranslation);

	SAND_TABLE_INTERNAL_CALL(InputIsKeyDown);
}

SAND_TABLE_NAMESPACE_END
