#pragma once

SAND_TABLE_NAMESPACE_BEGIN
struct Ray;
class RayTracingCamera
{
public:
	RayTracingCamera(float verticalFOV,
		float nearClip,
		float farClip,
		float fStep0 = 0,
		float fStep1 = 0);

	bool OnUpdate(float ts);
	void OnResize(uint32_t width, uint32_t height);

	const glm::mat4& GetProjection() const;
	const glm::mat4& GetInverseProjection() const;
	const glm::mat4& GetView() const;
	const glm::mat4& GetInverseView() const;

	void SetPosition(const glm::vec3& position);
	const glm::vec3& GetPosition() const;
	void SetForwardDirection(const glm::vec3& direction);
	const glm::vec3& GetForwardDirection() const;

	Ray GenRay(float u, float v) const;

	bool GetDirty();
	void ResetDirty();
private:
	void RecalculateProjection();
	void RecalculateView();
	void RecalculateRayDirections();

private:
	glm::mat4 m_matProjection;
	glm::mat4 m_matView;
	glm::mat4 m_matInverseProjection;
	glm::mat4 m_matInverseView;

	float m_fVerticalFOV;
	float m_fNearClip;
	float m_fFarClip;

	glm::vec3 m_vec3Position;
	glm::vec3 m_vec3ForwardDirection;
	glm::vec3 m_vec3UpDirection;
	glm::vec3 m_vec3RightDirection;


	glm::vec2 m_vec2LastMousePosition;

	uint32_t m_uiViewportWidth;
	uint32_t m_uiViewportHeight;

	float m_fStep0;
	float m_fStep1;

	bool m_bDirty;
};

SAND_TABLE_NAMESPACE_END
