#pragma once
#include "Ray.h"
SAND_TABLE_NAMESPACE_BEGIN

struct HitRecord
{
	double HitDistance;
	glm::dvec3 WorldPosition;
	glm::dvec3 WorldNormal;
	uint32_t EntityID;
	uint32_t MaterialID;
	glm::dvec2 UV;
	bool FrontFace;
	void SetWorldNormal(const Ray& ray, const glm::dvec3& vec3WorldNormal)
	{
		FrontFace = glm::dot(ray.Direction, vec3WorldNormal) < 0;
		WorldNormal = FrontFace ? vec3WorldNormal : -vec3WorldNormal;
	}
};

class Hittable
{
public:
	virtual bool Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord) = 0;
};
SAND_TABLE_NAMESPACE_END
