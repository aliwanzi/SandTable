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
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void BeginNewFrame();
		void EndNewFrame();
	private:
		bool m_bShowDemoWindow;
		float m_fTime;
	};
}


