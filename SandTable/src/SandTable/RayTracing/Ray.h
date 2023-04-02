#pragma once
SAND_TABLE_NAMESPACE_BEGIN

struct Ray
{
	glm::dvec3 Origin;
	glm::dvec3 Direction;
	double Step;
	Ray() :Origin(glm::dvec3(0.0)), Direction(glm::dvec3(0.0)), Step(0.0)
	{

	}
	Ray(const glm::dvec3& origin, const glm::dvec3& dir, double step = 0.0) :
		Origin(origin), Direction(dir), Step(step)
	{

	}
};

SAND_TABLE_NAMESPACE_END

