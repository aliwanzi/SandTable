#include "pch.h"
#include "PlatForm/OpenGL/OpenGLShader.h"
#include "SandTable/Render/RenderAPI.h"

SAND_TABLE_NAMESPACE_BEGIN

Ref<Shader> Shader::Create(const std::vector<ShaderInfo>& vecShaderInfo, const std::string& sShaderName)
{
	switch (RenderAPI::GetAPIType())
	{
	case RenderAPI::APIType::OpenGL:
		return Ref<OpenGLShader>(new OpenGLShader(vecShaderInfo, sShaderName));
	default:
		SAND_TABLE_ASSERT(false, "Unknown RenderAPI");
		return nullptr;
	}
}

std::string Shader::ReadFile(const std::string& sFilepath)
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
			return sResult;
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
	return "";
}

void ShaderLibrary::Add(const Ref<Shader>& spShader)
{
	const auto& sShaderName = spShader->GetName();
	Add(spShader, sShaderName);
}

void ShaderLibrary::Add(const Ref<Shader>& spShader, const std::string& sShaderName)
{
	if (m_mapShaders.find(sShaderName) != m_mapShaders.end())
	{
		LOG_DEV_WARN("Shader already exists!");
	}
	m_mapShaders[sShaderName] = spShader;
}

Ref<Shader> ShaderLibrary::Load(const std::vector<ShaderInfo>& vecShaderInfo, const std::string& sShaderName)
{
	if (m_mapShaders.find(sShaderName) != m_mapShaders.end())
	{
		LOG_DEV_WARN("Shader already exists!");
	}
	auto spShader = Shader::Create(vecShaderInfo, sShaderName);
	m_mapShaders[sShaderName] = spShader;
	return spShader;
}

Ref<Shader> ShaderLibrary::Get(const std::string& sShaderName)
{
	SAND_TABLE_ASSERT(m_mapShaders.find(sShaderName) != m_mapShaders.end(), "Shader already exists!");

	return m_mapShaders[sShaderName];
}

SAND_TABLE_NAMESPACE_END


