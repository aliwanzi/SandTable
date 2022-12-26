#pragma once
#include "SandTable/Render/Camera/OrthoGraphicCamera.h"
#include "SandTable/Events/ApplicationEvent.h"
#include "SandTable/Events/KeyEvent.h"
#include "SandTable/Events/MouseEvent.h"
#include "SandTable/Core/TimeStep.h"

SAND_TABLE_NAMESPACE_BEGIN

struct OrthoGraphicCameraBounds
{
	float Left;
	float Right;
	float Bottom;
	float Top;
	float GetWidth() { return Right - Left; }
	float GetHeight() { return Top - Bottom; }
};

class OrthoGraphicCameraController
{
public:
	OrthoGraphicCameraController(float fAspectRatio, bool bRotation = false);
	const std::shared_ptr<OrthoGraphicCamera>& GetCamera()const;
	void SetZoomLevel(float fZoomLevel);
	float GetZoomLevel()const;
	void OnUpdate(TimeStep timeStep);
	void OnEvent(Event& event);
	OrthoGraphicCameraBounds GetOrthoGraphicCameraBounds();
private:
	bool OnMouseScrolled(MouseScrolledEvent& event);
	bool OnWindowResized(WindowResizedEvent& event);
private:
	Ref<OrthoGraphicCamera> m_spOrthoGraphicCamera;

	float m_fAspectRatio;
	float m_fZoomLevel;

	bool m_bRotation;
	float m_fCameraRotation;
	glm::vec3 m_vec3CameraPosition;

	float m_fCameraTranslationSpeed;
	float m_fCameraRotationSpeed;
	float m_fCameraZoomSpeed;
};


SAND_TABLE_NAMESPACE_END
