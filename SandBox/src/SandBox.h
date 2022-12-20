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

	Ref<VertexArray> m_spTextureArray;
	Ref<Shader> m_spTextureShader;

	Ref<VertexArray> m_spColorArray;
	Ref<Shader> m_spColorShader;
	glm::vec3 m_vec3Color;

	Ref<Texture> m_spTexture2DRGB, m_spTexture2DRGBA;
};