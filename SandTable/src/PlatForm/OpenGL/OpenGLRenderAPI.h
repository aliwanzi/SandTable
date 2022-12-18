#pragma once
#include "SandTable/Render/RenderAPI.h"
namespace SandTable
{
	class OpenGLRenderAPI:public RenderAPI
	{
	public:
		// Í¨¹ý RenderAPI ¼Ì³Ð
		virtual void SetClearColor(const glm::vec4& vec4Color) override;
		virtual void Clear() override;
		virtual void DrawVertex(const std::shared_ptr<VertexArray>& spVertexArray) override;
	};
}


