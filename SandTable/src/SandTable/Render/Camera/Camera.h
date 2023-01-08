#pragma once
#include <glm/gtc/matrix_transform.hpp>

SAND_TABLE_NAMESPACE_BEGIN

enum class ProjectionType
{
	Perspective = 0,
	Orthographic
};
static std::array<std::string, 2> ProjectionTypeStrings = { "Perspective","Orthographic" };

class Camera
{
public:
	Camera(float fNear, float fFar, ProjectionType eProjectionType);
	~Camera() = default;
	void SetPosition(const glm::vec3& vec3Position);
	glm::vec3& GetPositon();
	const glm::vec3& GetPositon()const;
	void SetRotation(float fRotation);
	const float GetRotaion()const;
	const glm::mat4& GetProjectionMatrix()const;
	const glm::mat4& GetViewMatrix()const;
	const glm::mat4& GetViewProjectionMatrix()const;
	ProjectionType GetProjectionType();
	void SetProjectionType(ProjectionType eProjectionType);

	void SetNearAndFarClip(float fNear, float fFar);
	void SetNearClip(float fNear);
	float GetNearClip();
	void SetFarClip(float fFar);
	float GetFarClip();

protected:
	virtual void RecalculateViewMatrix() = 0;
	virtual void RecalculateProjectionMatrix() = 0;
protected:
	ProjectionType m_eProjectionType;
	glm::mat4 m_mat4ProjectionMatrix;

	glm::mat4 m_mat4ViewMatrix;
	glm::mat4 m_mat4ViewProjectionMatrix;
	glm::vec3 m_vec3Position;
	float m_fRotation;

	float m_fNearClip;
	float m_fFarClip;
};

SAND_TABLE_NAMESPACE_END
