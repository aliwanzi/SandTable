#pragma once
#include "SandTable/Scene/Components.h"
#include "box2d/b2_world.h"

SAND_TABLE_NAMESPACE_BEGIN

enum class BodyType
{
	Static = 0,
	Dynamic,
	Kinematic
};

const std::array<std::string, 3> BodyTypeStrings = { "Static","Dynamic","Kinematic" };

struct RigidBody2DComponent
{
	BodyType Type = BodyType::Static;
	bool FixedRotation = false;
	void* RuntimeBody;

	RigidBody2DComponent() = default;
	RigidBody2DComponent(const RigidBody2DComponent&) = default;
};

struct BoxCollider2DComponent
{
	glm::vec2 Offset = glm::vec2(0.f);
	glm::vec2 Size = glm::vec2(0.5f);

	float Density = 1.f;
	float Friction = 0.5f;
	float Restitution = 0.f;
	float RestitutionThreshold = 0.5f;

	BoxCollider2DComponent() = default;
	BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
};

struct CircleCollider2DComponent
{
	glm::vec2 Offset = glm::vec2(0.f);
	float Radius = 0.5f;

	float Density = 1.f;
	float Friction = 0.5f;
	float Restitution = 0.f;
	float RestitutionThreshold = 0.5f;

	CircleCollider2DComponent() = default;
	CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
};

class PhysicsSystem2D
{
public:
	PhysicsSystem2D();
	~PhysicsSystem2D();
	void CreateBody(RigidBody2DComponent& rigidBody2D, const TransformComponent& transform);
	void CreatePolygonShape(BoxCollider2DComponent& boxCollider2D, const TransformComponent& transform);
	void CreateCircleShape(CircleCollider2DComponent& circleCollider2D, const TransformComponent& transform);

	void OnUpdate(const TimeStep& timeStep);
	void UpdateSystem(RigidBody2DComponent& rigidBody2D, TransformComponent& transform);
private:
	Ref<b2World> m_spPhysicsWorld;
	b2Body* m_pBody;
};

SAND_TABLE_NAMESPACE_END

