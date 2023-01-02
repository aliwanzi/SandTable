#pragma once
#include "SandTable.h"
#include <imgui/imgui.h>
using namespace SandTable;

class SandBoxEditorLayer:public Layer
{
public:
	SandBoxEditorLayer();
	~SandBoxEditorLayer();
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(const TimeStep& timeStep) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Event& e) override;
private:
	Ref<OrthoGraphicCameraController> m_spOrthoGraphicCameraController;
	Ref<Texture> m_spTexture;
	Ref<Texture> m_spTextureStar;

	Ref<Texture> m_spTextureSprite;
	Ref<SubTexture2D> m_spSubTexStairs, m_spSubTexBarrel, m_spSubTexTree;

	glm::vec4 m_vec4Color;

	Ref<ParticleSystem2D> m_spParticleSystem2D;
	ParticleProps m_Particle;

	Ref<Buffer> m_spFrameBuffer;

	bool m_bRenderWindowActive;
};

