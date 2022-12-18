#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace SandTable
{
	class Camera
	{
	public:
		Camera(const glm::mat4& mat4ProjectionMatrix);
		void SetPosition(const glm::vec3& vec3Position);
		const glm::vec3& GetPositon()const;
		void SetRotation(float fRotation);
		const float GetRotaion()const;
		const glm::mat4& GetProjectionMatrix()const;
		const glm::mat4& GetViewMatrix()const;
		const glm::mat4& GetViewProjectionMatrix()const;
	protected:
		virtual void RecalculateViewMatrix() = 0;
	protected:
		glm::mat4 m_mat4ProjectionMatrix;
		glm::mat4 m_mat4ViewMatrix;
		glm::mat4 m_mat4ViewProjectionMatrix;
		glm::vec3 m_vec3Position;
		float m_fRotation;
	};
}

