#include "pch.h"
#include "OrthoGraphicCamera.h"

SAND_TABLE_NAMESPACE_BEGIN

OrthoGraphicCamera::OrthoGraphicCamera(float fLeft, float fRight, float fBottom, float fTop) :
	Camera(glm::ortho(fLeft, fRight, fBottom, fTop))
{

}

void OrthoGraphicCamera::SetProjection(float fLeft, float fRight, float fBottom, float fTop)
{
	m_mat4ProjectionMatrix = glm::ortho(fLeft, fRight, fBottom, fTop);
	m_mat4ViewProjectionMatrix = m_mat4ProjectionMatrix * m_mat4ViewMatrix;
}

void OrthoGraphicCamera::RecalculateViewMatrix()
{
	auto mat4Transform = glm::translate(glm::mat4(1.f), m_vec3Position);
	mat4Transform = glm::rotate(mat4Transform, glm::radians(m_fRotation), glm::vec3(0.f, 0.f, 1.f));

	m_mat4ViewMatrix = mat4Transform;
	m_mat4ViewProjectionMatrix = m_mat4ProjectionMatrix * m_mat4ViewMatrix;
}

SAND_TABLE_NAMESPACE_END