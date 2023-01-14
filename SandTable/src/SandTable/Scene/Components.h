#pragma once
#include "SandTable/Render/Camera/OrthoGraphicCamera.h"
#include "SandTable/Render/Camera/PerspectiveGraphicCamera.h"
#include "SandTable/Scene/Entity.h"
#include "SandTable/Scene/ScriptableEntity.h"

SAND_TABLE_NAMESPACE_BEGIN

struct TagComponent
{
	std::string Tag = "Entity";
	TagComponent() = default;
	TagComponent(const TagComponent&) = default;
	TagComponent& operator =(const TagComponent&) = default;
	TagComponent(const std::string& sTag) :Tag(sTag)
	{}
};

struct TransformComponent
{
	glm::vec3 Translation = glm::vec3(0.f);
	glm::vec3 Rotation = glm::vec3(0.f);
	glm::vec3 Scale = glm::vec3(1.f);
	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent& operator =(const TransformComponent&) = default;
	TransformComponent(const glm::vec3 & translation, const glm::vec3 & rotation, const glm::vec3 & scale) :
		Translation(translation),
		Rotation(rotation),
		Scale(scale)
	{}

	operator glm::mat4() 
	{
		glm::mat4 mat4Translation = glm::translate(glm::mat4(1.f), Translation);

		glm::mat4 mat4Rotation = glm::rotate(glm::mat4(1.f), Rotation.x, glm::vec3(1.0, 0.0, 0.0)) *
			glm::rotate(glm::mat4(1.f), Rotation.y, glm::vec3(0.0, 1.0, 0.0)) *
			glm::rotate(glm::mat4(1.f), Rotation.z, glm::vec3(0.0, 0.0, 1.0));

		glm::mat4 mat4Scale = glm::scale(glm::mat4(1.f), Scale);

		return mat4Translation * mat4Rotation * mat4Scale;
	}
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
	Ref<SandTable::Camera> OrthoCamera = CreateRef<OrthoGraphicCamera>();
	Ref<SandTable::Camera> PerspecCamera = CreateRef<PerspectiveGraphicCamera>();
	bool Primary = true;
	bool FixedAspectRatio = false;
	ProjectionType Projection = ProjectionType::Orthographic;
	CameraComponent() = default;
	CameraComponent(const CameraComponent&) = default;
	CameraComponent& operator =(const CameraComponent&) = default;
	CameraComponent(const Ref<SandTable::Camera>& spCamera, bool bPrimary = true, bool bFixedAspectRatio = false)
		:OrthoCamera(spCamera), Primary(bPrimary), FixedAspectRatio(bFixedAspectRatio)
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
