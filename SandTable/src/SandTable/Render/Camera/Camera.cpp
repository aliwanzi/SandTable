#include "pch.h"
#include "Camera.h"

SAND_TABLE_NAMESPACE_BEGIN

Camera::Camera(float fAspectRatio, float fNear, float fFar, ProjectionType eProjectionType) :
	m_mat4ProjectionMatrix(glm::mat4(1.f)),
	m_mat4ViewMatrix(glm::mat4(1.f)),
	m_vec3Position(glm::vec3(0.f)),
	m_quatRotation(glm::quat(0.f,0.f,0.f,0.f)),
	m_fAspectRatio(fAspectRatio),
	m_fNearClip(fNear),
	m_fFarClip(fFar),
	m_eProjectionType(eProjectionType),
	m_uiWidth(0),
	m_uiHeight(0)
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

void Camera::SetRotation(const glm::quat& quatRotation)
{
	m_quatRotation = quatRotation;
}

const glm::quat& Camera::GetRotaion()const
{
	return m_quatRotation;
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

void Camera::SetViewPortSize(unsigned int uiWidth, unsigned int uiHeight)
{
	if (m_uiWidth != uiWidth || m_uiHeight != uiHeight)
	{
		m_uiWidth = uiWidth;
		m_uiHeight = uiHeight;
		m_fAspectRatio = static_cast<float>(m_uiWidth) / static_cast<float>(m_uiHeight);
		RecalculateProjectionMatrix();
	}
}

void Camera::SetAspectRatio(float fAspectRatio)
{
	m_fAspectRatio = fAspectRatio;
	RecalculateProjectionMatrix();
}

float Camera::GetAspectRatio()
{
	return m_fAspectRatio;
}

void Camera::RecalculateViewMatrix()
{
	glm::mat4 mat4Translation = glm::translate(glm::mat4(1.f), m_vec3Position);

	glm::mat4 mat4Rotation = glm::toMat4(m_quatRotation);

	m_mat4ViewMatrix = glm::inverse(mat4Translation * mat4Rotation);

	RecalculateViewProjectionMatrix();
}

void Camera::RecalculateViewProjectionMatrix()
{
	m_mat4ViewProjectionMatrix = m_mat4ProjectionMatrix * m_mat4ViewMatrix;
}

SAND_TABLE_NAMESPACE_END
