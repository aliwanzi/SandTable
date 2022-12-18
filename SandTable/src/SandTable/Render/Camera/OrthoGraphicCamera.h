#pragma once
#include <glm/glm.hpp>
#include "SandTable/Render/Camera/Camera.h"

namespace SandTable
{
	class OrthoGraphicCamera:public Camera
	{
	public:
		OrthoGraphicCamera(float fLeft, float fRight, float fBottom, float fTop);
	private:
		virtual void RecalculateViewMatrix() override;
	};
}


