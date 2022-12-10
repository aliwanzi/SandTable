#pragma once
#include "Core.h"
#include "SandTable/Events/Event.h"

namespace SandTable
{
	class SAND_TABLE_API Layer
	{
	public:
		Layer(const std::string& sLayerName = "Layer") :m_sLayerName(sLayerName) {}
		virtual ~Layer() {};
		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnEvent(Event& event) = 0;
		const std::string& GetName()const { return m_sLayerName; }
	protected:
		std::string m_sLayerName;
	};
}