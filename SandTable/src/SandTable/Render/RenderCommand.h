#pragma once
#include "SandTable/Render/RenderAPI.h"

namespace SandTable
{
	class RenderCommand
	{
	public:
		static void SetClearColor(const glm::vec4& vec4Color);
		static void Clear();
		static void DrawVertex(const std::shared_ptr<VertexArray>& spVertexArray);
	private:
		static std::shared_ptr<RenderAPI> m_spRenderAPI;
	};
}


