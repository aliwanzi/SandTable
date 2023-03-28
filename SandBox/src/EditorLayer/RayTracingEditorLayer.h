#pragma once
#include "SandTable.h"
#include <imgui/imgui.h>


class RayTracingEditorLayer :public Layer
{
public:
	RayTracingEditorLayer();
	virtual void OnAttach() override;
	virtual void OnUpdate(const TimeStep&) override;
	virtual void OnImGuiRender() override;
private:
	void CreatEarthSphereScene();
	void CreatTwoSphereScene();
	void CreatFourSphereScene();
	void CreateMultiSphereScene();
	void CreateSampleLightScene();
	void CreateCornellBoxScene();
	void CreateCornellBoxSmokeScene();
	void CreateFinalScene();
private:

	ImVec2 m_vec2RenderViewPortSize;

	Ref<ObjectContainer> m_spObjectContainer;
	Ref<RayTracingScene> m_spRayTracingScene;
	Ref<RayTracingCamera> m_spRayTracingCamera;

	Ref<Timer> m_spTimer;
	float m_fLastRenderTime;
};

