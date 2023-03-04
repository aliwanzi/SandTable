#include "pch.h"
#include "ScriptGlue.h"

#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"

#include "box2d/b2_body.h"

#include "SandTable/Core/KeyCode.h"
#include "SandTable/Core/Input.h"

#include "SandTable/Scene/UUID.h"
#include "SandTable/Scene/Scene.h"

#include "SandTable/Script/ScriptEngine.h"


SAND_TABLE_NAMESPACE_BEGIN

namespace
{
	static std::unordered_map<MonoType*, std::function<bool(Ref<Entity>)>> mapEntityHasComponentFuncs;

	template<typename Component>
	void RegisterComponent()
	{
		std::string sTypeName = typeid(Component).name();
		size_t iPos = sTypeName.find_last_of(':');
		std::string sStructName = sTypeName.substr(iPos + 1);
		std::string sComponentName = fmt::format("SandTable.{}", sStructName);

		MonoType* pMonoType = mono_reflection_type_from_name(sComponentName.data(), ScriptEngine::GetCoreAssemblyImage().get());
		if (pMonoType == nullptr)
		{
			LOG_DEV_ERROR("could not find component {}", sComponentName);
		}
		mapEntityHasComponentFuncs[pMonoType] = [](Ref<Entity> spEntity)
		{
			return spEntity->HasComponent<TransformComponent>();
		};
	}

	bool EntityHasComponet(UUID entityID,MonoReflectionType* componentType)
	{
		MonoType* pMonoType = mono_reflection_type_get_type(componentType);

		auto spEntity = ScriptEngine::GetRuntimeScene()->GetEntityByUUID(entityID);
		auto iter = mapEntityHasComponentFuncs.find(pMonoType);
		if (spEntity != nullptr && iter != mapEntityHasComponentFuncs.end())
		{
			return iter->second(spEntity);
		}
		return false;
	}

	uint64_t EntityFindEntityByName(MonoString* sEntityName)
	{
		char* pEntityName = mono_string_to_utf8(sEntityName);
		
		auto spScene = ScriptEngine::GetRuntimeScene();
		auto spEntity = spScene->GetEntityByName(pEntityName);
		mono_free(pEntityName);
		if (spEntity == nullptr)
		{
			return 0;
		}
		else
		{
			return spEntity->GetUUID();
		}
	}

	MonoObject* GetScriptEntityInstance(UUID entityID)
	{
		return ScriptEngine::GetMonoObject(entityID);
	}

	void TransformComponentGetTranslation(UUID entityID, glm::vec3* vec3Par)
	{
		auto spScene = ScriptEngine::GetRuntimeScene();
		auto spEntity = spScene->GetEntityByUUID(entityID);
		*vec3Par = spEntity->GetComponent<TransformComponent>().Translation;
	}

	void TransformComponentSetTranslation(UUID entityID, glm::vec3* vec3Par)
	{
		auto spScene = ScriptEngine::GetRuntimeScene();
		auto spEntity = spScene->GetEntityByUUID(entityID);
		spEntity->GetComponent<TransformComponent>().Translation = *vec3Par;
	}

	void RigidBody2DComponentApplyLinearImpulse(UUID entityID,glm::vec2* vec2Impulse,glm::vec2* vec2Point,bool bWake)
	{
		auto spScene = ScriptEngine::GetRuntimeScene();
		auto spEntity = spScene->GetEntityByUUID(entityID);
		auto& pRuntimeBody = spEntity->GetComponent<RigidBody2DComponent>();
		b2Body* pB2Body = static_cast<b2Body*>(pRuntimeBody.RuntimeBody);
		pB2Body->ApplyLinearImpulse(b2Vec2(vec2Impulse->x, vec2Impulse->y), b2Vec2(vec2Point->x, vec2Point->y), bWake);
	}

	void RigidBody2DComponentApplyLinearImpulseToCenter(UUID entityID, glm::vec2* vec2Impulse, bool bWake)
	{
		auto spScene = ScriptEngine::GetRuntimeScene();
		auto spEntity = spScene->GetEntityByUUID(entityID);
		auto& pRuntimeBody = spEntity->GetComponent<RigidBody2DComponent>();
		b2Body* pB2Body = static_cast<b2Body*>(pRuntimeBody.RuntimeBody);
		pB2Body->ApplyLinearImpulseToCenter(b2Vec2(vec2Impulse->x, vec2Impulse->y), bWake);;
	}

	bool InputIsKeyDown(KeyCode keyCode)
	{
		return Input::IsKeyPressed(keyCode);
	}

#define SAND_TABLE_INTERNAL_CALL(Name) mono_add_internal_call("SandTable.InternalCall::" #Name, Name)
}

void ScriptGlue::RegisterFunctions()
{  
	SAND_TABLE_INTERNAL_CALL(EntityHasComponet);
	SAND_TABLE_INTERNAL_CALL(EntityFindEntityByName);
	SAND_TABLE_INTERNAL_CALL(GetScriptEntityInstance);

	SAND_TABLE_INTERNAL_CALL(TransformComponentGetTranslation);
	SAND_TABLE_INTERNAL_CALL(TransformComponentSetTranslation);

	SAND_TABLE_INTERNAL_CALL(RigidBody2DComponentApplyLinearImpulse);
	SAND_TABLE_INTERNAL_CALL(RigidBody2DComponentApplyLinearImpulseToCenter);

	SAND_TABLE_INTERNAL_CALL(InputIsKeyDown);
}

void ScriptGlue::RegisterComponents()
{
	RegisterComponent<TransformComponent>();
	RegisterComponent<SpriteRenderComponent>();
	RegisterComponent<CircleRenderComponent>();
	RegisterComponent<CameraComponent>();
	RegisterComponent<RigidBody2DComponent>();
	RegisterComponent<BoxCollider2DComponent>();
	RegisterComponent<CircleCollider2DComponent>();
	RegisterComponent<ScriptComponent>();
}

SAND_TABLE_NAMESPACE_END
