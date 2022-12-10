#pragma once
#include "SandTable/Core/Core.h"
#include "SandTable/Core/Layer.h"

namespace SandTable
{
	class SAND_TABLE_API ImGuiLayer :public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& event) override;
	private:
		bool m_bShowDemoWindow;
		float m_fTime;
	};
}


