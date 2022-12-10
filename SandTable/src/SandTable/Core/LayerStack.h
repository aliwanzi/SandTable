#pragma once
#include "Core.h"
#include "Layer.h"
namespace SandTable
{
	class SAND_TABLE_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(std::shared_ptr<Layer> spLayer);
		void PushOverlay(std::shared_ptr<Layer> spLayer);
		void PopLayer(std::shared_ptr<Layer> spLayer);
		void PopOverlay(std::shared_ptr<Layer> spLayer);
		const std::list<std::shared_ptr<Layer>>& GetLayers()const;
	private:
		std::list<std::shared_ptr<Layer>> m_listLayers;
	};
}


