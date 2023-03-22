#include "pch.h"
#include "RayTracingCamera.h"
#include "SandTable/Core/TimeStep.h"
#include "SandTable/Core/Input.h"
#include "SandTable/Core/KeyCode.h"
#include "SandTable/Core/MouseCode.h"

SAND_TABLE_NAMESPACE_BEGIN

RayTracingCamera::RayTracingCamera(float verticalFOV, float nearClip, float farClip)
	: m_VerticalFOV(verticalFOV), m_NearClip(nearClip), m_FarClip(farClip),m_bDirty(true)
{
	m_ForwardDirection = glm::vec3(0, 0, -1);
	m_Position = glm::vec3(0, 0,2);
}

bool RayTracingCamera::OnUpdate(float ts)
{
	glm::vec2 mousePos = Input::GetMousePos();
	glm::vec2 delta = (mousePos - m_LastMousePosition) * 0.002f;
	m_LastMousePosition = mousePos;

	if (!Input::IsMouseButtonPressed(Mouse::ButtonRight))
	{
		Input::SetCursorMode(CursorMode::Normal);
		return false;
	}
	Input::SetCursorMode(CursorMode::Locked);

	bool moved = false;

	constexpr glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
	glm::vec3 rightDirection = glm::cross(m_ForwardDirection, upDirection);

	float speed = 5.0f;

	// Movement
	if (Input::IsKeyPressed(Key::W))
	{
		m_Position += m_ForwardDirection * speed * ts;
		moved = true;
	}
	else if (Input::IsKeyPressed(Key::S))
	{
		m_Position -= m_ForwardDirection * speed * ts;
		moved = true;
	}
	if (Input::IsKeyPressed(Key::A))
	{
		m_Position -= rightDirection * speed * ts;
		moved = true;
	}
	else if (Input::IsKeyPressed(Key::D))
	{
		m_Position += rightDirection * speed * ts;
		moved = true;
	}
	if (Input::IsKeyPressed(Key::Q))
	{
		m_Position += upDirection * speed * ts;
		moved = true;
	}
	else if (Input::IsKeyPressed(Key::E))
	{
		m_Position -= upDirection * speed * ts;
		moved = true;
	}

	// Rotation
	if (delta.x != 0.0f || delta.y != 0.0f)
	{
		float pitchDelta = delta.y * GetRotationSpeed();
		float yawDelta = delta.x * GetRotationSpeed();

		glm::quat q = glm::normalize(glm::cross(glm::angleAxis(-pitchDelta, rightDirection),
			glm::angleAxis(-yawDelta, glm::vec3(0.f, 1.0f, 0.0f))));
		m_ForwardDirection = glm::rotate(q, m_ForwardDirection);

		moved = true;
	}

	if (moved)
	{
		RecalculateView();
		//RecalculateRayDirections();
	}

	return moved;
}

void RayTracingCamera::OnResize(uint32_t width, uint32_t height)
{
	if (width == m_ViewportWidth && height == m_ViewportHeight)
		return;

	LOG_DEV_INFO("RayTracingCamera::OnResize");
	m_ViewportWidth = width;
	m_ViewportHeight = height;

	RecalculateProjection();
	//RecalculateRayDirections();
}

void RayTracingCamera::SetPosition(const glm::vec3& position)
{
	m_Position = position;
	RecalculateView();
}

void RayTracingCamera::SetForwardDirection(const glm::vec3& direction)
{
	m_ForwardDirection = direction;
	RecalculateView();
}

float RayTracingCamera::GetRotationSpeed()
{
	return 0.3f;
}

bool RayTracingCamera::GetDirty()
{
	return m_bDirty;
}

void RayTracingCamera::ResetDirty()
{
	m_bDirty = false;
}

void RayTracingCamera::RecalculateProjection()
{
	m_Projection = glm::perspectiveFov(glm::radians(m_VerticalFOV), (float)m_ViewportWidth, (float)m_ViewportHeight, m_NearClip, m_FarClip);
	m_InverseProjection = glm::inverse(m_Projection);
	m_bDirty = true;
}

void RayTracingCamera::RecalculateView()
{
	m_View = glm::lookAt(m_Position, m_Position + m_ForwardDirection, glm::vec3(0, 1, 0));
	m_InverseView = glm::inverse(m_View);
	m_bDirty = true;
}

void RayTracingCamera::RecalculateRayDirections()
{
	m_RayDirections.resize(m_ViewportWidth * m_ViewportHeight);

	for (uint32_t y = 0; y < m_ViewportHeight; y++)
	{
		for (uint32_t x = 0; x < m_ViewportWidth; x++)
		{
			glm::vec2 coord = { (float)x / (float)m_ViewportWidth, (float)y / (float)m_ViewportHeight };
			coord = coord * 2.0f - 1.0f; // -1 -> 1

			glm::vec4 target = m_InverseProjection * glm::vec4(coord.x, coord.y, 1, 1);
			glm::vec3 rayDirection = glm::vec3(m_InverseView * glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0)); // World space
			m_RayDirections[x + y * m_ViewportWidth] = rayDirection;
		}
	}
}

SAND_TABLE_NAMESPACE_END