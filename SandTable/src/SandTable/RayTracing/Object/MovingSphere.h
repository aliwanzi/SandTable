#pragma once
#include "Sphere.h"
SAND_TABLE_NAMESPACE_BEGIN
class MovingSphere :public Sphere
{
public:
	MovingSphere(uint32_t uiEntitID);

	void SetStepBegin(double fStepBegin);
	void SetStepEnd(double fStepEnd);

	void SetMovePosition(const glm::dvec3& vec3MovePosition);
	const glm::dvec3& GetMovePosition()const;

	// ͨ�� Hittable �̳�
	virtual bool Hit(const Ray& ray, float fMin, float fMax, HitRecord& hitRecord) const override;
private:
	glm::dvec3 Center(float fStep) const;
private:
	glm::dvec3 m_vec3MovePosition;
	double m_fStepBegin;
	double m_fStepEnd;
};

SAND_TABLE_NAMESPACE_END