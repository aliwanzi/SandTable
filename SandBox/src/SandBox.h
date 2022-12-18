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
	std::shared_ptr<OrthoGraphicCamera> m_spOrthoGraphicCamera;
	glm::vec3 m_vec3CameraPosition;
	float m_fCameraMoveSpeed;

	float m_fCameraRotation;
	float m_fCameraRotateSpeed;

	std::shared_ptr<VertexArray> m_spTriangle;
	std::shared_ptr<Shader> m_spTriangleShader;

	std::shared_ptr<VertexArray> m_spSquare;
	std::shared_ptr<Shader> m_spSquareShader;

	glm::vec3 m_vec3Color;
};