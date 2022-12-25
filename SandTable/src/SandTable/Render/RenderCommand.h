#pragma once
#include "SandTable/Render/RenderAPI.h"

SAND_TABLE_NAMESPACE_BEGIN

class RenderCommand
{
public:
	static void Init();
	static void SetClearColor(const glm::vec4& vec4Color);
	static void SetViewPort(unsigned int uiWidth, unsigned int uiHeight);
	static void Clear();
	static void DrawVertex(const Ref<VertexArray>& spVertexArray, uint32_t uiIndexCount = 0);
private:
	static Ref<RenderAPI> m_spRenderAPI;
};

SAND_TABLE_NAMESPACE_END