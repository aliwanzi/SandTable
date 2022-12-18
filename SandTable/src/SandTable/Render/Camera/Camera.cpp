#include "pch.h"
#include "Camera.h"

namespace SandTable
{
	Camera::Camera(const glm::mat4& mat4ProjectionMatrix) :
		m_mat4ProjectionMatrix(mat4ProjectionMatrix),
		m_mat4ViewMatrix(glm::mat4(1.f)),
		m_vec3Position(glm::vec3(0.f)),
		m_fRotation(0.f)
	{
		m_mat4ViewProjectionMatrix = m_mat4ProjectionMatrix * m_mat4ViewMatrix;
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
}
