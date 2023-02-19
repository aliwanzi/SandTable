#pragma once
#include "SandTable/Render/Camera/OrthoGraphicCamera.h"
#include "SandTable/Render/Camera/PerspectiveGraphicCamera.h"
#include "SandTable/Render/Texture/Texture.h"
#include "SandTable/Scene/UUID.h"
#include "SandTable/Scene/Primitive/QuadPrimitive.h"
#include "SandTable/Scene/Primitive/CirclePrimitive.h"
#include "SandTable/Scene/Primitive/LinePrimitive.h"

SAND_TABLE_NAMESPACE_BEGIN

struct IDComponent
{
	UUID ID;
	IDComponent() = default;
	IDComponent(const IDComponent&) = default;
	IDComponent& operator =(const IDComponent&) = default;
};

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

	glm::mat4 GetTransform() const
	{
		glm::mat4 mat4Translation = glm::translate(glm::mat4(1.f), Translation);

		glm::mat4 mat4Rotation = glm::toMat4(glm::quat(Rotation));

		glm::mat4 mat4Scale = glm::scale(glm::mat4(1.f), Scale);

		return mat4Translation * mat4Rotation * mat4Scale;
	}
};

struct SpriteRenderComponent
{
	Ref<Texture> spTexture;
	Ref<QuadPrimitive> spQuadPrimitive = CreateRef<QuadPrimitive>();
	SpriteRenderComponent() = default;
	SpriteRenderComponent(const SpriteRenderComponent&) = default;
	SpriteRenderComponent& operator =(const SpriteRenderComponent&) = default;
};

struct CircleRenderComponent
{
	Ref<Texture> spTexture;
	Ref<CirclePrimitive> spCirclePrimitive = CreateRef<CirclePrimitive>();
	CircleRenderComponent() = default;
	CircleRenderComponent(const CircleRenderComponent&) = default;
	CircleRenderComponent& operator =(const CircleRenderComponent&) = default;
};

struct CameraComponent
{
	Ref<Camera> OrthoCamera = CreateRef<OrthoGraphicCamera>();
	Ref<Camera> PerspecCamera = CreateRef<PerspectiveGraphicCamera>();
	bool Primary = true;
	bool FixedAspectRatio = false;
	ProjectionType Projection = ProjectionType::Orthographic;
	CameraComponent() = default;
	CameraComponent(const CameraComponent&) = default;
	CameraComponent& operator =(const CameraComponent&) = default;
	Ref<SandTable::Camera> GetCamera()
	{
		switch (Projection)
		{
			case SandTable::ProjectionType::Perspective:
			{
				return PerspecCamera;
			}
			case SandTable::ProjectionType::Orthographic:
			{
				return OrthoCamera;
			}
		}
		return nullptr;
	}
};

struct ScriptComponent
{
	std::string ClassName;
	ScriptComponent() = default;
	ScriptComponent(const ScriptComponent&) = default;
	ScriptComponent& operator =(const ScriptComponent&) = default;
};
SAND_TABLE_NAMESPACE_END
