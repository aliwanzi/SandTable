#include "pch.h"
#include "PlatForm/OpenGL/OpenGLShader.h"
#include "SandTable/Render/RenderAPI.h"

namespace SandTable
{
	std::shared_ptr<Shader> Shader::Create(const std::vector<ShaderInfo>& vecShaderInfo)
	{
		switch (RenderAPI::GetAPIType())
		{
		case RenderAPI::APIType::OpenGL:
			return std::shared_ptr<OpenGLShader>(new OpenGLShader(vecShaderInfo));
		default:
			SAND_TABLE_ASSERT(false, "Unknown RenderAPI");
			return nullptr;
		}
	}

	const char* Shader::ReadFile(const std::string& sFilepath)
	{
		std::ifstream ifSteam(sFilepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
		if (ifSteam)
		{
			ifSteam.seekg(0, std::ios::end);
			size_t stSize = ifSteam.tellg();
			if (stSize != -1)
			{
				std::string sResult("");
				sResult.resize(stSize);
				ifSteam.seekg(0, std::ios::beg);
				ifSteam.read(&sResult[0], stSize);
				return sResult.c_str();
			}
			else
			{
				LOG_DEV_ERROR("Could not read from file '{0}'", sFilepath);
			}
		}
		else
		{
			LOG_DEV_ERROR("Could not open file '{0}'", sFilepath);
		}
		return nullptr;
	}

	Shader::Shader():m_uiRenderID(0)
	{

	}
}