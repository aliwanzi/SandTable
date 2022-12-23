#pragma once
#include <glm/glm.hpp>
#include "SandTable/Render/Camera/Camera.h"

SAND_TABLE_NAMESPACE_BEGIN

class OrthoGraphicCamera :public Camera
{
public:
	OrthoGraphicCamera(float fLeft, float fRight, float fBottom, float fTop);
	void SetProjection(float fLeft, float fRight, float fBottom, float fTop);
private:
	virtual void RecalculateViewMatrix() override;
};

SAND_TABLE_NAMESPACE_END
