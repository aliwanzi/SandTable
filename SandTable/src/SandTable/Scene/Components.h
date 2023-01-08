#pragma once
#include "SandTable/Render/Camera/Camera.h"
#include "SandTable/Scene/Entity.h"
#include "SandTable/Scene/ScriptableEntity.h"

SAND_TABLE_NAMESPACE_BEGIN

struct TagComponent
{
	std::string Tag;
	TagComponent() = default;
	TagComponent(const TagComponent&) = default;
	TagComponent& operator =(const TagComponent&) = default;
	TagComponent(const std::string& sTag) :Tag(sTag)
	{}
};

struct TransformComponent
{
	glm::mat4 Transform{ 1.f };
	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent& operator =(const TransformComponent&) = default;
	TransformComponent(const glm::mat4& mat4Transform) :Transform(mat4Transform)
	{}

	operator glm::mat4& () { return Transform; }
	operator const glm::mat4& ()const { return Transform; }
};


struct SpriteRenderComponent
{
	glm::vec4 Color{ 1.f };
	SpriteRenderComponent() = default;
	SpriteRenderComponent(const SpriteRenderComponent&) = default;
	SpriteRenderComponent& operator =(const SpriteRenderComponent&) = default;
	SpriteRenderComponent(const glm::vec4& vec4Color) :Color(vec4Color)
	{}
	operator glm::vec4& () { return Color; }
	operator const glm::vec4& ()const { return Color; }
};

struct CameraComponent
{
	Ref<SandTable::Camera> Camera;
	bool Primary;
	bool FixedAspectRation;
	CameraComponent() = default;
	CameraComponent(const CameraComponent&) = default;
	CameraComponent& operator =(const CameraComponent&) = default;
	CameraComponent(const Ref<SandTable::Camera>& spCamera, bool bPrimary = true, bool bFixedAspectRation = false)
		:Camera(spCamera), Primary(bPrimary), FixedAspectRation(bFixedAspectRation)
	{}
};

struct NativeScriptComponent
{
	Ref<ScriptableEntity> Instance;

	std::function<void()> InstantiateFunction;

	template<typename T>
	void Bind()
	{
		InstantiateFunction = [&]()
		{
			Instance = CreateRef<T>();
		};
	}
};

SAND_TABLE_NAMESPACE_END
