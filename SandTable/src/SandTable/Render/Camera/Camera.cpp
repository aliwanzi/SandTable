#include "pch.h"
#include "Camera.h"

SAND_TABLE_NAMESPACE_BEGIN

Camera::Camera(float fNear, float fFar, ProjectionType eProjectionType) :
	m_mat4ProjectionMatrix(glm::mat4(1.f)),
	m_mat4ViewMatrix(glm::mat4(1.f)),
	m_vec3Position(glm::vec3(0.f)),
	m_fRotation(0.f),
	m_fNearClip(fNear),
	m_fFarClip(fFar),
	m_eProjectionType(eProjectionType)
{
}

void Camera::SetPosition(const glm::vec3& vec3Position)
{
	m_vec3Position = vec3Position;
	RecalculateViewMatrix();
}

glm::vec3& Camera::GetPositon()
{
	return m_vec3Position;
}

const glm::vec3& Camera::GetPositon() const
{
	return m_vec3Position;
}

void Camera::SetRotation(float fRotation)
{
	m_fRotation = fRotation;
	RecalculateViewMatrix();
}

const float Camera::GetRotaion() const
{
	return m_fRotation;
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
	return m_mat4ProjectionMatrix;
}

const glm::mat4& Camera::GetViewMatrix() const
{
	return m_mat4ViewMatrix;
}

const glm::mat4& Camera::GetViewProjectionMatrix() const
{
	return m_mat4ViewProjectionMatrix;
}

ProjectionType Camera::GetProjectionType()
{
	return m_eProjectionType;
}

void Camera::SetProjectionType(ProjectionType eProjectionType)
{
	m_eProjectionType = eProjectionType;
}

void Camera::SetNearAndFarClip(float fNear, float fFar)
{
	m_fNearClip = fNear;
	m_fFarClip = fFar;
	RecalculateProjectionMatrix();
}

void Camera::SetNearClip(float fNear)
{
	m_fNearClip = fNear;
	RecalculateProjectionMatrix();
}

float Camera::GetNearClip()
{
	return m_fNearClip;
}

void Camera::SetFarClip(float fFar)
{
	m_fFarClip = fFar;
	RecalculateProjectionMatrix();
}

float Camera::GetFarClip()
{
	return m_fFarClip;
}

SAND_TABLE_NAMESPACE_END
