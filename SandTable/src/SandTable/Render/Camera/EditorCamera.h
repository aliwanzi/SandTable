#pragma once
#include "SandTable/Render/Camera/PerspectiveGraphicCamera.h"

SAND_TABLE_NAMESPACE_BEGIN
class TimeStep;
class Event;
class MouseScrolledEvent;

class EditorCamera :public PerspectiveGraphicCamera
{
public:
	EditorCamera(float fFOV, float fAspectRatio, float fNearClip = 0.1f, float fFarClip = 1000.f);
	void OnUpdate(const TimeStep& ts);
	void OnEvent(Event& e);

	float GetDistance();
	void SetDistance(float fDistance);

	glm::vec3 GetUpDirection();
	glm::vec3 GetRightDirection();
	glm::vec3 GetForwardDirection();

	glm::quat GetOrientation();

	float GetPitch() const;
	float GetYaw() const;

private:
	void UpdateView();

	bool OnMouseScroll(MouseScrolledEvent& e);

	void MousePan(const glm::vec2& delta);
	void MouseRotate(const glm::vec2& delta);
	void MouseZoom(float delta);

	glm::vec3 CalculatePosition();

	glm::vec2 PanSpeed();
	float RotationSpeed();
	float ZoomSpeed();

private:
	float m_fDistance;
	float m_fPitch;
	float m_fYaw;
	glm::vec3 m_vec3FocalPoint;
	glm::vec2 m_vec2MousePosition;
};

SAND_TABLE_NAMESPACE_END

