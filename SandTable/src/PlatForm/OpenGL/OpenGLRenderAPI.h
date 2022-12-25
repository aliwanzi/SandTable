#pragma once
#include "SandTable/Render/RenderAPI.h"

SAND_TABLE_NAMESPACE_BEGIN

class OpenGLRenderAPI :public RenderAPI
{
public:
	// ͨ�� RenderAPI �̳�
	virtual void Init()override;
	virtual void SetClearColor(const glm::vec4& vec4Color) override;
	virtual void SetViewPort(unsigned int uiWidth, unsigned int uiHeight) override;
	virtual void Clear() override;
	virtual void DrawVertex(const Ref<VertexArray>& spVertexArray, uint32_t uiIndexCount = 0) override;
};

SAND_TABLE_NAMESPACE_END