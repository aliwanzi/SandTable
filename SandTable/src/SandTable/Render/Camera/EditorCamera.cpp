#include "pch.h"
#include "SandTable/Render/Camera/EditorCamera.h"
#include "SandTable/Events/MouseEvent.h"
#include "SandTable/Core/TimeStep.h"
#include "SandTable/Core/Input.h"
#include "SandTable/Core/KeyCode.h"
#include "SandTable/Core/MouseCode.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

SAND_TABLE_NAMESPACE_BEGIN
EditorCamera::EditorCamera(float fFOV, float fAspectRatio, float fNearClip, float fFarClip) :
	PerspectiveGraphicCamera(fFOV, fAspectRatio, fNearClip, fFarClip),
	m_fDistance(10.f),
	m_fPitch(0.f),
	m_fYaw(0.f),
	m_vec3FocalPoint(glm::vec3(0.f)),
	m_vec2MousePosition(glm::vec2(0.f))
{
	UpdateView();
}

void EditorCamera::OnUpdate(const TimeStep& ts)
{
	if (Input::IsKeyPressed(Key::LeftAlt))
	{
		glm::vec2 delta = (Input::GetMousePos() - m_vec2MousePosition) * 0.003f;
		m_vec2MousePosition = Input::GetMousePos();

		if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
			MousePan(delta);
		else if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
			MouseRotate(delta);
		else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
			MouseZoom(delta.y);
	}
	UpdateView();
}

void EditorCamera::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FUN(EditorCamera::OnMouseScroll));
}

float EditorCamera::GetDistance()
{
	return m_fDistance;
}

void EditorCamera::SetDistance(float fDistance)
{
	m_fDistance = fDistance;
}

glm::vec3 EditorCamera::GetUpDirection()
{
	return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 EditorCamera::GetRightDirection()
{
	return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 EditorCamera::GetForwardDirection()
{
	return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::quat EditorCamera::GetOrientation()
{
	return glm::quat(glm::vec3(-m_fPitch, -m_fYaw, 0.0f));
}

float EditorCamera::GetPitch() const
{
	return m_fPitch;
}

float EditorCamera::GetYaw() const
{
	return m_fYaw;
}

void EditorCamera::UpdateView()
{
	m_vec3Position = CalculatePosition();
	m_quatRotation = GetOrientation();
	RecalculateViewMatrix();
}

bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
{
	float fDelta = e.GetYOffset() * 0.1f;
	MouseZoom(fDelta);
	return false;
}

void EditorCamera::MousePan(const glm::vec2& delta)
{
	m_vec3FocalPoint += -GetRightDirection() * delta.x * PanSpeed().x * m_fDistance;
	m_vec3FocalPoint += GetUpDirection() * delta.y * PanSpeed().y * m_fDistance;
}

void EditorCamera::MouseRotate(const glm::vec2& delta)
{
	float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
	m_fYaw += yawSign * delta.x * RotationSpeed();
	m_fPitch += delta.y * RotationSpeed();
}

void EditorCamera::MouseZoom(float delta)
{
	m_fDistance -= delta * ZoomSpeed();
	if (m_fDistance < 1.0f)
	{
		m_vec3FocalPoint += GetForwardDirection();
		m_fDistance = 1.0f;
	}
}

glm::vec3 EditorCamera::CalculatePosition()
{
	return m_vec3FocalPoint - GetForwardDirection() * m_fDistance;
}

glm::vec2 EditorCamera::PanSpeed()
{
	float x = std::min(m_uiWidth / 1000.0f, 2.4f); // max = 2.4f
	float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

	float y = std::min(m_uiHeight / 1000.0f, 2.4f); // max = 2.4f
	float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

	return glm::vec2(xFactor, yFactor);
}

float EditorCamera::RotationSpeed()
{
	return 0.8f;
}

float EditorCamera::ZoomSpeed()
{
	float distance = std::max(m_fDistance * 0.2f, 0.0f);
	return std::min(distance * distance, 100.0f); // max speed = 100;
}

SAND_TABLE_NAMESPACE_END