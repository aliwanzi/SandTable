#pragma once
#include "SandTable/Core/Layer.h"

SAND_TABLE_NAMESPACE_BEGIN

class LayerStack
{
public:
	LayerStack() = default;
	~LayerStack();
	void PushLayer(Ref<Layer> spLayer);
	void PushOverlay(Ref<Layer> spLayer);
	void PopLayer(Ref<Layer> spLayer);
	void PopOverlay(Ref<Layer> spLayer);
	const std::list<Ref<Layer>>& GetLayers()const;
private:
	std::list<Ref<Layer>> m_listLayers;
};

SAND_TABLE_NAMESPACE_END