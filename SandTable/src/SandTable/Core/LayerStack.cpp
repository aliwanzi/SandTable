#include "pch.h"
#include "LayerStack.h"

namespace SandTable
{
	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
	}

	void LayerStack::PushLayer(std::shared_ptr<Layer> spLayer)
	{
		m_listLayers.push_back(spLayer);
	}

	void LayerStack::PushOverlay(std::shared_ptr<Layer> spLayer)
	{
		m_listLayers.push_back(spLayer);
	}

	void LayerStack::PopLayer(std::shared_ptr<Layer> spLayer)
	{
		m_listLayers.pop_back();
	}

	void LayerStack::PopOverlay(std::shared_ptr<Layer> spLayer)
	{
		m_listLayers.pop_back();
	}

	const std::list<std::shared_ptr<Layer>>& LayerStack::GetLayers() const
	{
		return m_listLayers;
	}
}
