#pragma once
#include <glm/glm.hpp>
#include "SandTable/Render/VertexArray.h"

namespace SandTable
{
	class RenderAPI
	{
	public:
		enum class APIType
		{
			None = 0, OpenGL
		};
		virtual void SetClearColor(const glm::vec4& vec4Color) = 0;
		virtual void Clear() = 0;
		virtual void DrawVertex(const std::shared_ptr<VertexArray>& spVertexArray) = 0;
		static APIType GetAPIType();
		static std::shared_ptr<RenderAPI> CreateRenderAPI();

	protected:
		RenderAPI() = default;
		RenderAPI(const RenderAPI&) = delete;
		RenderAPI& operator=(const RenderAPI&) = delete;
	};
}


