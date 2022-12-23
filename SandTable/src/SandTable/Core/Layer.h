#pragma once
#include "SandTable/Events/Event.h"
#include "SandTable/Core/TimeStep.h"

SAND_TABLE_NAMESPACE_BEGIN

class Layer
{
public:
	Layer(const std::string& sLayerName = "Layer") :m_sLayerName(sLayerName) {}
	virtual ~Layer() {};
	virtual void OnAttach() {};
	virtual void OnDetach() {};
	virtual void OnUpdate(const TimeStep&) {};
	virtual void OnEvent(Event&) {};
	virtual void OnImGuiRender() {}
	const std::string& GetName()const { return m_sLayerName; }
protected:
	std::string m_sLayerName;
};

SAND_TABLE_NAMESPACE_END