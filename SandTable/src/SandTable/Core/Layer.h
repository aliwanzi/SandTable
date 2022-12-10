#pragma once
#include "Core.h"
#include "SandTable/Events/Event.h"

namespace SandTable
{
	class SAND_TABLE_API Layer
	{
	public:
		Layer(const std::string& sLayerName = "Layer");
		virtual ~Layer();
		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnUpdate();
		virtual void OnEvent(Event& event);
		const std::string& GetName()const;
	protected:
		std::string m_sLayerName;
	};
}