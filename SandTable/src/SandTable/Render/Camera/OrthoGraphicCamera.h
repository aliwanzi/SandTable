#pragma once
#include "SandTable/Render/Camera/Camera.h"

SAND_TABLE_NAMESPACE_BEGIN

class OrthoGraphicCamera :public Camera
{
public:
	~OrthoGraphicCamera() = default;
	OrthoGraphicCamera(float fSize, float fAspectRatio, float fNearClip = -1.f, float fFarClip = 1.f);
	void SetProjection(float fLeft, float fRight, float fBottom, float fTop);
	void SetOrthoSize(float fSize);
	void SetViewPortSize(unsigned int uiWidth, unsigned int uiHeight);

private:
	virtual void RecalculateViewMatrix() override;
	virtual void RecalculateProjectionMatrix() override;
private:
	float m_fOrthoGraphicSize;
	float m_fAspectRatio;
};

SAND_TABLE_NAMESPACE_END
