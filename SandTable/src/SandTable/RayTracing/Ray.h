#pragma once
SAND_TABLE_NAMESPACE_BEGIN

struct Ray
{
	glm::vec3 Origin;
	glm::vec3 Direction;
};

struct HitPayLoad
{
	float HitDistance;
	glm::vec3 WorldPosition;
	glm::vec3 WorldNormal;
	uint32_t EntityID;
	bool FrontFace;
	void SetWorldNormal(const Ray& ray,const glm::vec3& vec3WorldNormal)
	{
		FrontFace = glm::dot(ray.Direction, vec3WorldNormal) < 0;
		WorldNormal = FrontFace ? glm::normalize(vec3WorldNormal) : -glm::normalize(vec3WorldNormal);
	}
};

SAND_TABLE_NAMESPACE_END

