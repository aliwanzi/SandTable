#include "pch.h"
#include "Camera.h"

SAND_TABLE_NAMESPACE_BEGIN

Camera::Camera(float fAspectRatio, float fNear, float fFar, ProjectionType eProjectionType) :
	m_mat4ProjectionMatrix(glm::mat4(1.f)),
	m_mat4ViewMatrix(glm::mat4(1.f)),
	m_vec3Position(glm::vec3(0.f)),
	m_fRotation(0.f),
	m_fAspectRatio(fAspectRatio),
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

const glm::vec3& Camera::GetPositon() const
{
	return m_vec3Position;
}

void Camera::SetRotation(float fRotation)
{
	m_fRotation = fRotation;
	RecalculateViewMatrix();
}

float Camera::GetRotaion() const
{
	return m_fRotation;
}

void Camera::SetProjectionMatrix(const glm::mat4& mat4ProjectionMatrix)
{
	m_mat4ProjectionMatrix = mat4ProjectionMatrix;
	RecalculateViewProjectionMatrix();
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
	return m_mat4ProjectionMatrix;
}

void Camera::SetViewMatrix(const glm::mat4& mat4ViewMatrix)
{
	m_mat4ViewMatrix = mat4ViewMatrix;
	RecalculateViewProjectionMatrix();
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

float Camera::GetNearClip() const
{
	return m_fNearClip;
}

void Camera::SetFarClip(float fFar)
{
	m_fFarClip = fFar;
	RecalculateProjectionMatrix();
}

float Camera::GetFarClip() const
{
	return m_fFarClip;
}

void Camera::RecalculateViewMatrix()
{
	auto mat4Transform = glm::translate(glm::mat4(1.f), m_vec3Position);
	mat4Transform = glm::rotate(mat4Transform, glm::radians(m_fRotation), glm::vec3(0.f, 0.f, 1.f));

	m_mat4ViewMatrix = mat4Transform;
	RecalculateViewProjectionMatrix();
}

void Camera::RecalculateViewProjectionMatrix()
{
	m_mat4ViewProjectionMatrix = m_mat4ProjectionMatrix * m_mat4ViewMatrix;
}

SAND_TABLE_NAMESPACE_END
