#include "pch.h"
#include "RayTracingCamera.h"
#include "SandTable/Core/TimeStep.h"
#include "SandTable/Core/Input.h"
#include "SandTable/Core/KeyCode.h"
#include "SandTable/Core/MouseCode.h"
#include "SandTable/RayTracing/Ray.h"
#include "SandTable/Math/Random.h"

SAND_TABLE_NAMESPACE_BEGIN
namespace
{
	const float fRotationSpeed = 0.3;
	const float fMoveSpeed = 0.002f;
}

RayTracingCamera::RayTracingCamera(float verticalFOV,
	float nearClip,
	float farClip,
	float fStep0,
	float fStep1) :
	m_fVerticalFOV(verticalFOV),
	m_fNearClip(nearClip),
	m_fFarClip(farClip),
	m_fStep0(fStep0),
	m_fStep1(fStep1),
	m_vec3Position(glm::vec3(0.f)),
	m_vec3ForwardDirection(glm::vec3(0.f, 0.f, -1.f)),
	m_vec2LastMousePosition(glm::vec2(0.f)),
	m_uiViewportWidth(0),
	m_uiViewportHeight(0),
	m_matProjection(glm::mat4(1.f)),
	m_matView(glm::mat4(1.f)),
	m_matInverseProjection(glm::mat4(1.f)),
	m_matInverseView(glm::mat4(1.f)),
	m_bDirty(true)
{

}

bool RayTracingCamera::OnUpdate(float ts)
{
	glm::vec2 mousePos = Input::GetMousePos();
	glm::vec2 delta = (mousePos - m_vec2LastMousePosition) * fMoveSpeed;
	m_vec2LastMousePosition = mousePos;

	if (!Input::IsMouseButtonPressed(Mouse::ButtonRight))
	{
		Input::SetCursorMode(CursorMode::Normal);
		return false;
	}
	Input::SetCursorMode(CursorMode::Locked);

	bool moved = false;

	constexpr glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
	m_vec3RightDirection = glm::cross(m_vec3ForwardDirection, upDirection);
	m_vec3UpDirection = glm::cross(m_vec3RightDirection, m_vec3ForwardDirection);

	float speed = 5.0f;

	// Movement
	if (Input::IsKeyPressed(Key::W))
	{
		m_vec3Position += m_vec3ForwardDirection * speed * ts;
		moved = true;
	}
	else if (Input::IsKeyPressed(Key::S))
	{
		m_vec3Position -= m_vec3ForwardDirection * speed * ts;
		moved = true;
	}
	if (Input::IsKeyPressed(Key::A))
	{
		m_vec3Position -= m_vec3RightDirection * speed * ts;
		moved = true;
	}
	else if (Input::IsKeyPressed(Key::D))
	{
		m_vec3Position += m_vec3RightDirection * speed * ts;
		moved = true;
	}
	if (Input::IsKeyPressed(Key::Q))
	{
		m_vec3Position += m_vec3UpDirection * speed * ts;
		moved = true;
	}
	else if (Input::IsKeyPressed(Key::E))
	{
		m_vec3Position -= m_vec3UpDirection * speed * ts;
		moved = true;
	}

	// Rotation
	if (glm::abs(delta.x) > 0.0f || glm::abs(delta.y) > 0.0f)
	{
		float pitchDelta = delta.y * fRotationSpeed;
		float yawDelta = delta.x * fRotationSpeed;

		glm::quat q = glm::normalize(glm::cross(glm::angleAxis(-pitchDelta, m_vec3RightDirection),
			glm::angleAxis(-yawDelta, glm::vec3(0.f, 1.0f, 0.0f))));
		m_vec3ForwardDirection = glm::rotate(q, m_vec3ForwardDirection);

		moved = true;
	}

	if (moved)
	{
		RecalculateView();
	}

	return moved;
}

void RayTracingCamera::OnResize(uint32_t width, uint32_t height)
{
	if (width == m_uiViewportWidth && height == m_uiViewportHeight)
		return;

	LOG_DEV_INFO("RayTracingCamera::OnResize with is {0}, height is {1}", width, height);
	m_uiViewportWidth = width;
	m_uiViewportHeight = height;
	RecalculateProjection();
}

void RayTracingCamera::SetPosition(const glm::vec3& position)
{
	m_vec3Position = position;
	RecalculateView();
}

const glm::vec3& RayTracingCamera::GetPosition() const
{
	return m_vec3Position;
}

void RayTracingCamera::SetForwardDirection(const glm::vec3& direction)
{
	m_vec3ForwardDirection = direction;
	RecalculateView();
}

const glm::vec3& RayTracingCamera::GetForwardDirection() const
{
	return m_vec3ForwardDirection;
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
	m_matProjection = glm::perspectiveFov(glm::radians(m_fVerticalFOV), (float)m_uiViewportWidth, (float)m_uiViewportHeight,
		m_fNearClip, m_fFarClip);
	m_matInverseProjection = glm::inverse(m_matProjection);

	m_bDirty = true;
}

void RayTracingCamera::RecalculateView()
{
	m_matView = glm::lookAt(m_vec3Position, m_vec3Position + m_vec3ForwardDirection, glm::vec3(0, 1, 0));
	m_matInverseView = glm::inverse(m_matView);
	m_bDirty = true;
}

Ray RayTracingCamera::GenRay(float u, float v) const
{
	Ray ray;
	ray.Origin = m_vec3Position;
	ray.Step = m_fStep0 + (m_fStep1 - m_fStep0) * Random::Float();
	glm::vec2 vec2Coord = { static_cast<float>(u) / static_cast<float>(m_uiViewportWidth),
		static_cast<float>(v) / static_cast<float>(m_uiViewportHeight) };

	vec2Coord = vec2Coord * 2.f - 1.f; //(0~1)->(-1,1)
	glm::vec4 target = m_matInverseProjection * glm::vec4(vec2Coord.x, vec2Coord.y, 1, 1);
	ray.Direction = glm::vec3(m_matInverseView * glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0));
	return ray;
}

const glm::mat4& RayTracingCamera::GetProjection() const
{
	return m_matProjection;
}

const glm::mat4& RayTracingCamera::GetInverseProjection() const
{
	return m_matInverseProjection;
}

const glm::mat4& RayTracingCamera::GetView() const
{
	return m_matView;
}

const glm::mat4& RayTracingCamera::GetInverseView() const
{
	return m_matInverseView;
}


SAND_TABLE_NAMESPACE_END