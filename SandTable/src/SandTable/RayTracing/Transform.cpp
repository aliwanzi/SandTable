#include "pch.h"
#include "Transform.h"

SAND_TABLE_NAMESPACE_BEGIN
Transform::Transform():
	m_fRotateY(0.f),
	m_vec3Translate(glm::vec3(0.f)),
	m_mat4Rotation(glm::highp_dmat4(1.f)),
	m_mat4RotationInverse(glm::highp_dmat4(1.f))
{
}

void Transform::SetTranslate(const glm::dvec3& vec3Translate)
{
	m_vec3Translate = vec3Translate;
}

const glm::dvec3& Transform::GetTranslate() const
{
	return m_vec3Translate;
}

const glm::highp_dmat4& Transform::GetRotation() const
{
	return m_mat4Rotation;
}

const glm::highp_dmat4& Transform::GetRotationInverse() const
{
	return m_mat4RotationInverse;
}

void Transform::SetRotateY(double fAngle)
{
	m_fRotateY = fAngle;
	m_mat4Rotation = glm::rotate(glm::highp_dmat4(1.), fAngle, glm::dvec3(0., 1., 0.));
	m_mat4RotationInverse = glm::inverse(m_mat4Rotation);
}

double Transform::GetRotateY() const
{
	return m_fRotateY;
}

SAND_TABLE_NAMESPACE_END
