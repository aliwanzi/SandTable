#pragma once
#include "SandTable/Render/Camera/Camera.h"

SAND_TABLE_NAMESPACE_BEGIN

class PerspectiveGraphicCamera :public Camera
{
public:
	PerspectiveGraphicCamera();
	PerspectiveGraphicCamera(float fFOV, float fAspectRatio, float fNearClip = 0.1f, float fFarClip = 1000.f);
	void SetPerspectiveFOV(float fFOV);
	float GetPerspectiveFOV();
private:
	void RecalculateProjectionMatrix() override;
private:
	float m_fFOV;
};

SAND_TABLE_NAMESPACE_END

