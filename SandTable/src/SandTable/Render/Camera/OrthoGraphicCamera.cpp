#include "pch.h"
#include "OrthoGraphicCamera.h"

SAND_TABLE_NAMESPACE_BEGIN

OrthoGraphicCamera::OrthoGraphicCamera(float fSize, float fAspectRatio, float fNearClip, float fFarClip) :
	m_fOrthoGraphicSize(fSize),
	m_fAspectRatio(fAspectRatio),
	Camera(fNearClip, fFarClip, ProjectionType::Orthographic)
{
	RecalculateProjectionMatrix();
}

void OrthoGraphicCamera::SetProjection(float fLeft, float fRight, float fBottom, float fTop)
{
	m_mat4ProjectionMatrix = glm::ortho(fLeft, fRight, fBottom, fTop, m_fNearClip, m_fFarClip);
	m_mat4ViewProjectionMatrix = m_mat4ProjectionMatrix * m_mat4ViewMatrix;
}

void OrthoGraphicCamera::SetOrthoSize(float fSize)
{
	m_fOrthoGraphicSize = fSize;
	RecalculateProjectionMatrix();
}

void OrthoGraphicCamera::SetViewPortSize(unsigned int uiWidth, unsigned int uiHeight)
{
	m_fAspectRatio = static_cast<float>(uiWidth) / static_cast<float>(uiHeight);
	RecalculateProjectionMatrix();
}

void OrthoGraphicCamera::RecalculateViewMatrix()
{
	auto mat4Transform = glm::translate(glm::mat4(1.f), m_vec3Position);
	mat4Transform = glm::rotate(mat4Transform, glm::radians(m_fRotation), glm::vec3(0.f, 0.f, 1.f));

	m_mat4ViewMatrix = mat4Transform;
	m_mat4ViewProjectionMatrix = m_mat4ProjectionMatrix * m_mat4ViewMatrix;
}

void OrthoGraphicCamera::RecalculateProjectionMatrix()
{
	float fOrthoLeft = -m_fOrthoGraphicSize * m_fAspectRatio * 0.5f;
	float fOrthoRight = m_fOrthoGraphicSize * m_fAspectRatio * 0.5f;
	float fOrthoBottom = -m_fOrthoGraphicSize * 0.5f;
	float fOrthoTop = m_fOrthoGraphicSize * 0.5f;
	m_mat4ProjectionMatrix = glm::ortho(fOrthoLeft, fOrthoRight, fOrthoBottom, fOrthoTop,
		m_fNearClip, m_fFarClip);
	m_mat4ViewProjectionMatrix = m_mat4ProjectionMatrix * m_mat4ViewMatrix;
}

SAND_TABLE_NAMESPACE_END