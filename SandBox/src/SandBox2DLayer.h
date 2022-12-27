#pragma once
#include "SandTable.h"
#include <imgui/imgui.h>
using namespace SandTable;

class SandBox2DLayer :public Layer
{
public:
	SandBox2DLayer();
	~SandBox2DLayer();
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(const TimeStep& timeStep) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Event& e) override;
private:
	struct ProfileResult
	{
		std::string Name;
		float Time;
	};
	Ref<OrthoGraphicCameraController> m_spOrthoGraphicCameraController;
	Ref<Texture> m_spTexture;
	Ref<Texture> m_spTextureStar;

	glm::vec4 m_vec4Color;

	std::vector<ProfileResult> m_vecProfileResult;

	Ref<ParticleSystem2D> m_spParticleSystem2D;
	ParticleProps m_Particle;
};

