#include "pch.h"
#include "Transform.h"

SAND_TABLE_NAMESPACE_BEGIN
Transform::Transform():
	m_fRotateY(0.f),
	m_vec3Translate(glm::vec3(0.f)),
	m_mat4RotationInverse(glm::highp_mat4(1.f)),
	m_mat4Rotation(glm::highp_dmat4(1.f)),
	m_mat4TranslationInverse(glm::highp_mat4(1.f)),
	m_mat4Translation(glm::highp_mat4(1.f))
	
{
}

void Transform::SetTranslate(const glm::dvec3& vec3Translate)
{
	m_vec3Translate = vec3Translate;
	m_mat4Translation = glm::translate(glm::mat4(1.f), glm::vec3(m_vec3Translate));
	m_mat4TranslationInverse = glm::inverse(m_mat4Translation);
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

const glm::highp_dmat4& Transform::GetTranslationInverse() const
{
	return m_mat4TranslationInverse;
}

const glm::highp_dmat4& Transform::GetTranslation() const
{
	return m_mat4Translation;
}

void Transform::SetRotateY(double fAngle)
{
	m_fRotateY = fAngle;
	m_mat4Rotation = glm::rotate(glm::highp_dmat4(1.), glm::radians(m_fRotateY), glm::dvec3(0., 1, 0.));
	m_mat4RotationInverse = glm::inverse(m_mat4Rotation);
}

double Transform::GetRotateY() const
{
	return m_fRotateY;
}

SAND_TABLE_NAMESPACE_END
