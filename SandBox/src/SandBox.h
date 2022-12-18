#pragma once
#include "SandTable.h"
#include <imgui/imgui.h>

using namespace SandTable;
class SandBoxLayer :public Layer
{
public:
	SandBoxLayer();
	virtual ~SandBoxLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(const TimeStep& timeStep) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Event& e) override;
private:
	Ref<OrthoGraphicCamera> m_spOrthoGraphicCamera;
	glm::vec3 m_vec3CameraPosition;
	float m_fCameraMoveSpeed;

	float m_fCameraRotation;
	float m_fCameraRotateSpeed;

	Ref<VertexArray> m_spTriangle;
	Ref<Shader> m_spTriangleShader;

	Ref<VertexArray> m_spSquare;
	Ref<Shader> m_spSquareShader;

	glm::vec3 m_vec3Color;
};