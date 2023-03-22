#pragma once
#include "Object.h"

SAND_TABLE_NAMESPACE_BEGIN
class Sphere :public Object
{
public:
	Sphere(uint32_t uiEntitID);

	void SetPosition(const glm::vec3& vec3Position);
	const glm::vec3& GetPosition() const;

	void SetRadius(float fRadius);
	float GetRadius() const;

	// Í¨¹ý Hittable ¼Ì³Ð
	virtual bool Hit(const Ray& ray, float fMin, float fMax, HitRecord& hitRecord) const override;

protected:
	glm::vec3 m_vec3Position;
	float m_fRadius;
};

SAND_TABLE_NAMESPACE_END

