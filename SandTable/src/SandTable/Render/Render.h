#pragma once
#include "SandTable/Render/VertexArray.h"
#include "SandTable/Render/Camera/Camera.h"
#include "SandTable/Render/Shader.h"

namespace SandTable
{
	class Render
	{
	public:
		static void BeginScene(const std::shared_ptr<Camera>& spCamera);
		static void EndScene();
		static void Submit(const std::shared_ptr<Shader>& spShader,
			const std::shared_ptr<VertexArray>& spVertexArray,
			const glm::mat4& mat4Transform = glm::mat4(1.0f));
	private:
		struct SceneData
		{
			glm::mat4 m_mat4ViewProjectionMatrix;
		};

		static std::shared_ptr<SceneData> m_spSceneData;
	};
}


