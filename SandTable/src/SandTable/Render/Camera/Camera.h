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
	Camera(float fAspectRatio, float fNear, float fFar, ProjectionType eProjectionType);
	~Camera() = default;
	void SetPosition(const glm::vec3& vec3Position);
	const glm::vec3& GetPositon()const;
	void SetRotation(const glm::quat& quatRotation);
	const glm::quat& GetRotaion()const;

	void SetProjectionMatrix(const glm::mat4& mat4ProjectionMatrix);
	const glm::mat4& GetProjectionMatrix()const;
	void SetViewMatrix(const glm::mat4& mat4ViewMatrix);
	const glm::mat4& GetViewMatrix()const;
	const glm::mat4& GetViewProjectionMatrix()const;
	ProjectionType GetProjectionType();
	void SetProjectionType(ProjectionType eProjectionType);

	void SetNearAndFarClip(float fNear, float fFar);
	void SetNearClip(float fNear);
	float GetNearClip() const;
	void SetFarClip(float fFar);
	float GetFarClip() const;

	void SetViewPortSize(unsigned int uiWidth, unsigned int uiHeight);
	void SetAspectRatio(float fAspectRatio);
	float GetAspectRatio();

protected:
	virtual void RecalculateProjectionMatrix() = 0;
	void RecalculateViewProjectionMatrix();
	void RecalculateViewMatrix();

protected:
	ProjectionType m_eProjectionType;
	glm::mat4 m_mat4ProjectionMatrix;

	glm::mat4 m_mat4ViewMatrix;
	glm::mat4 m_mat4ViewProjectionMatrix;
	glm::vec3 m_vec3Position;
	glm::quat m_quatRotation;

	float m_fNearClip;
	float m_fFarClip;
	float m_fAspectRatio;
	unsigned int m_uiWidth;
	unsigned int m_uiHeight;
};

SAND_TABLE_NAMESPACE_END
