#include "pch.h"
#include "LayerStack.h"

SAND_TABLE_NAMESPACE_BEGIN

LayerStack::LayerStack()
{
}

LayerStack::~LayerStack()
{
}

void LayerStack::PushLayer(Ref<Layer> spLayer)
{
	m_listLayers.emplace_front(spLayer);
}

void LayerStack::PushOverlay(Ref<Layer> spLayer)
{
	m_listLayers.emplace_back(spLayer);
}

void LayerStack::PopLayer(Ref<Layer> spLayer)
{
	m_listLayers.pop_back();
}

void LayerStack::PopOverlay(Ref<Layer> spLayer)
{
	m_listLayers.pop_back();
}

const std::list<Ref<Layer>>& LayerStack::GetLayers() const
{
	return m_listLayers;
}

SAND_TABLE_NAMESPACE_END
