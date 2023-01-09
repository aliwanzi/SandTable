#include "pch.h"
#include "OrthoGraphicCamera.h"

SAND_TABLE_NAMESPACE_BEGIN

OrthoGraphicCamera::OrthoGraphicCamera(float fSize, float fAspectRatio, float fNearClip, float fFarClip) :
	m_fOrthoGraphicSize(fSize),
	Camera(fAspectRatio, fNearClip, fFarClip, ProjectionType::Orthographic)
{
	RecalculateProjectionMatrix();
}

void OrthoGraphicCamera::SetOrthoSize(float fSize)
{
	m_fOrthoGraphicSize = fSize;
	RecalculateProjectionMatrix();
}

float OrthoGraphicCamera::GetOrthoSize() const
{
	return m_fOrthoGraphicSize;
}

void OrthoGraphicCamera::SetViewPortSize(unsigned int uiWidth, unsigned int uiHeight)
{
	m_fAspectRatio = static_cast<float>(uiWidth) / static_cast<float>(uiHeight);
	RecalculateProjectionMatrix();
}

void OrthoGraphicCamera::RecalculateProjectionMatrix()
{
	float fOrthoLeft = -m_fOrthoGraphicSize * m_fAspectRatio;
	float fOrthoRight = m_fOrthoGraphicSize * m_fAspectRatio;
	float fOrthoBottom = -m_fOrthoGraphicSize;
	float fOrthoTop = m_fOrthoGraphicSize;
	m_mat4ProjectionMatrix = glm::ortho(fOrthoLeft, fOrthoRight, fOrthoBottom, fOrthoTop,
		m_fNearClip, m_fFarClip);

	RecalculateViewProjectionMatrix();
}

SAND_TABLE_NAMESPACE_END