#pragma once
#include "Ray.h"
SAND_TABLE_NAMESPACE_BEGIN

struct HitRecord
{
	float HitDistance;
	glm::vec3 WorldPosition;
	glm::vec3 WorldNormal;
	uint32_t EntityID;
	uint32_t MaterialID;
	bool FrontFace;
	void SetWorldNormal(const Ray& ray, const glm::vec3& vec3WorldNormal)
	{
		FrontFace = glm::dot(ray.Direction, vec3WorldNormal) < 0;
		WorldNormal = FrontFace ? vec3WorldNormal : -vec3WorldNormal;
	}
};

class Hittable
{
public:
	virtual bool Hit(const Ray& ray, float fMin, float fMax, HitRecord& hitRecord) const = 0;
};
SAND_TABLE_NAMESPACE_END
