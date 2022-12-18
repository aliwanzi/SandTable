#pragma once
#include <glm/glm.hpp>
#include "SandTable/Render/VertexArray.h"

SAND_TABLE_NAMESPACE_BEGIN

class RenderAPI
{
public:
	enum class APIType
	{
		None = 0, OpenGL
	};
	virtual void SetClearColor(const glm::vec4& vec4Color) = 0;
	virtual void Clear() = 0;
	virtual void DrawVertex(const Ref<VertexArray>& spVertexArray) = 0;
	static APIType GetAPIType();
	static Ref<RenderAPI> CreateRenderAPI();

protected:
	RenderAPI() = default;
	RenderAPI(const RenderAPI&) = delete;
	RenderAPI& operator=(const RenderAPI&) = delete;
};

SAND_TABLE_NAMESPACE_END