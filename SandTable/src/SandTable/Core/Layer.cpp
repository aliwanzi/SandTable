#include "pch.h"
#include "Layer.h"

namespace SandTable
{
	Layer::Layer(const std::string& sLayerName) :m_sLayerName(sLayerName)
	{
	}

	Layer::~Layer()
	{
	}

	void Layer::OnAttach()
	{
	}

	void Layer::OnDetach()
	{
	}

	void Layer::OnUpdate()
	{
	}

	void Layer::OnEvent(Event& event)
	{
	}

	const std::string& Layer::GetName() const
	{
		return m_sLayerName;
	}
}

