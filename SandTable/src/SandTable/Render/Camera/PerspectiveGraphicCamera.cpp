#include "pch.h"
#include "PerspectiveGraphicCamera.h"

SAND_TABLE_NAMESPACE_BEGIN
PerspectiveGraphicCamera::PerspectiveGraphicCamera(float fFOV, float fAspectRatio, float fNearClip, float fFarClip) :
	Camera(fAspectRatio, fNearClip, fFarClip, ProjectionType::Perspective),
	m_fFOV(45.f)
{

}

void PerspectiveGraphicCamera::SetPerspectiveFOV(float fFOV)
{
	m_fFOV = fFOV;
}

float PerspectiveGraphicCamera::GetPerspectiveFOV()
{
	return m_fFOV;
}

void PerspectiveGraphicCamera::RecalculateProjectionMatrix()
{
	m_mat4ProjectionMatrix = glm::perspective(m_fFOV, m_fAspectRatio, m_fNearClip, m_fFarClip);
	RecalculateViewProjectionMatrix();
}

SAND_TABLE_NAMESPACE_END


