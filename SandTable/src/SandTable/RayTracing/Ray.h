#pragma once
#include "SandTable/Scene/Primitive/SpherePrimitive.h"

SAND_TABLE_NAMESPACE_BEGIN
struct HitPayLoad
{
	float HitDistance;
	glm::vec3 WorldPosition;
	glm::vec3 WorldNormal;
	uint32_t EntityID;
};

struct Ray
{
	glm::vec3 Origin;
	glm::vec3 Direction;
};
SAND_TABLE_NAMESPACE_END

