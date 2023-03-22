#pragma once
#include "Sphere.h"
SAND_TABLE_NAMESPACE_BEGIN
class MovingSphere :public Sphere
{
public:
	MovingSphere(uint32_t uiEntitID);

	void SetStepBegin(float fStepBegin);
	void SetStepEnd(float fStepEnd);

	void SetMovePosition(const glm::vec3& vec3MovePosition);
	const glm::vec3& GetMovePosition()const;

	// Í¨¹ý Hittable ¼Ì³Ð
	virtual bool Hit(const Ray& ray, float fMin, float fMax, HitRecord& hitRecord) const override;
private:
	glm::vec3 Center(float fStep) const;
private:
	glm::vec3 m_vec3MovePosition;
	float m_fStepBegin;
	float m_fStepEnd;
};

SAND_TABLE_NAMESPACE_END