#pragma once
#include "SandTable/Events/Event.h"
#include "SandTable/Core/TimeStep.h"

namespace SandTable
{
	class SAND_TABLE_API Layer
	{
	public:
		Layer(const std::string& sLayerName = "Layer") :m_sLayerName(sLayerName) {}
		virtual ~Layer() {};
		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate(const TimeStep& timeStep) {};
		virtual void OnEvent(Event& event) {};
		virtual void OnImGuiRender() {}
		const std::string& GetName()const { return m_sLayerName; }
	protected:
		std::string m_sLayerName;
	};
}