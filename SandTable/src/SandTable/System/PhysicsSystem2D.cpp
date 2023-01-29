#include "pch.h"
#include "SandTable/System/PhysicsSystem2D.h"
#include <box2d/b2_fixture.h>
#include <box2d/b2_shape.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>

SAND_TABLE_NAMESPACE_BEGIN

namespace
{
	const unsigned int uiVelocityIterations = 6;
	const unsigned int uiPositionIterations = 2;
	b2BodyType RigidBodyTypeToBox2D(BodyType bodyTyep)
	{
		switch (bodyTyep)
		{
			case BodyType::Static: return b2_staticBody;
			case BodyType::Dynamic: return b2_dynamicBody;
			case BodyType::Kinematic: return b2_kinematicBody;
			default: 
			{
				SAND_TABLE_ASSERT(false, "Unknown bodyType");
				return b2_staticBody;
			}
		}
	} 
}

PhysicsSystem2D::PhysicsSystem2D()
{
	m_spPhysicsWorld = CreateRef<b2World>(b2Vec2(0.f, -9.8f));
}

void PhysicsSystem2D::CreateBody(RigidBody2DComponent& rigidBody2D, const TransformComponent& transform)
{
	b2BodyDef bodyDef;
	bodyDef.type = RigidBodyTypeToBox2D(rigidBody2D.Type);
	bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
	bodyDef.angle = transform.Rotation.z;
	auto pB2Body = m_spPhysicsWorld->CreateBody(&bodyDef);
	pB2Body->SetFixedRotation(bodyDef.fixedRotation);
	rigidBody2D.RuntimeBody = pB2Body;
}

void PhysicsSystem2D::CreateBody(RigidBody2DComponent& rigidBody2D, BoxCollider2DComponent& boxCollider2D,
	const TransformComponent& transform)
{
	b2BodyDef bodyDef;
	bodyDef.type = RigidBodyTypeToBox2D(rigidBody2D.Type);
	bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
	bodyDef.angle = transform.Rotation.z;

	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(boxCollider2D.Size.x * transform.Scale.x, boxCollider2D.Size.y * transform.Scale.y);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &polygonShape;
	fixtureDef.density = boxCollider2D.Density;
	fixtureDef.friction = boxCollider2D.Friction;
	fixtureDef.restitution = boxCollider2D.Restitution;
	fixtureDef.restitutionThreshold = boxCollider2D.RestitutionThreshold;

	auto pB2Body = m_spPhysicsWorld->CreateBody(&bodyDef);
	pB2Body->SetFixedRotation(bodyDef.fixedRotation);
	pB2Body->CreateFixture(&fixtureDef);
	rigidBody2D.RuntimeBody = pB2Body;
}

void PhysicsSystem2D::OnUpdate(const TimeStep& timeStep)
{
	m_spPhysicsWorld->Step(timeStep, uiVelocityIterations, uiPositionIterations);
}

void PhysicsSystem2D::UpdateSystem(RigidBody2DComponent& rigidBody2D, TransformComponent& transform)
{
	b2Body* pB2Body = (b2Body*)rigidBody2D.RuntimeBody;
	transform.Translation.x = pB2Body->GetPosition().x;
	transform.Translation.y = pB2Body->GetPosition().y;
	transform.Rotation.z = pB2Body->GetAngle();
}


SAND_TABLE_NAMESPACE_END