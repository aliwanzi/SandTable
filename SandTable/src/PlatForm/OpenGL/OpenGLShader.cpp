#include "pch.h"
#include "OpenGLShader.h"
#include "GL/gl3w.h"
#include "SandTable/Core/Logger.h"
#include "SandTable/Debug/Instrumentor.h"

SAND_TABLE_NAMESPACE_BEGIN

namespace
{
	GLenum GetShaderType(ShaderType shaderType)
	{
		switch (shaderType)
		{
		case SandTable::ShaderType::VETEX_SHADER: return GL_VERTEX_SHADER;
		case SandTable::ShaderType::FRAGMENT_SHADER: return GL_FRAGMENT_SHADER;
		default:
		{
			SAND_TABLE_ASSERT(false, "Unknonw Shader Type");
			return 0;
		}
		}
	}
}

OpenGLShader::OpenGLShader(const std::vector<ShaderInfo>& vecShaderInfo,const std::string& sShaderName):
	m_sShaderName(sShaderName)
{
	SAND_TABLE_PROFILE_FUNCTION();
	GLuint uiProgram = glCreateProgram();
	std::vector<GLuint> vecShaderIDs;
	vecShaderIDs.reserve(vecShaderInfo.size());
	for (auto& shaderInfo : vecShaderInfo)
	{
		GLuint uiShader = glCreateShader(GetShaderType(shaderInfo.Type));
		vecShaderIDs.emplace_back(uiShader);
		auto pSource = ReadFile(shaderInfo.Path);
		SAND_TABLE_ASSERT(pSource, "Shader Source in OpenGLShader is null");
		glShaderSource(uiShader, 1, &pSource, NULL);

		glCompileShader(uiShader);
		if (!CheckCompileError(uiShader, CompileType::LINK_STATUS))
		{
			for (auto& shader : vecShaderIDs)
			{
				glDeleteShader(shader);
			}
			SAND_TABLE_ASSERT(false, "Shader Compile Failed");
		}
		glAttachShader(uiProgram, uiShader);
	}

	glLinkProgram(uiProgram);

	if (!CheckCompileError(uiProgram, CompileType::PROGRAM_STATUS))
	{
		for (auto& shader : vecShaderIDs)
		{
			glDeleteShader(shader);
		}
		SAND_TABLE_ASSERT(false, "Program Compile Failed");
	}

	for (auto& shader : vecShaderIDs)
	{
		glDeleteShader(shader);
	}

	m_uiRenderID = uiProgram;
}

OpenGLShader::~OpenGLShader()
{
	SAND_TABLE_PROFILE_FUNCTION();
	glDeleteProgram(m_uiRenderID);
}

bool OpenGLShader::CheckCompileError(unsigned int uiShader, CompileType compileType)
{
	switch (compileType)
	{
	case SandTable::CompileType::LINK_STATUS:
	{
		GLint iComplied(-1);
		glGetShaderiv(uiShader, GL_COMPILE_STATUS, &iComplied);
		if (!iComplied)
		{
			GLsizei uiLen(0);
			glGetShaderiv(uiShader, GL_INFO_LOG_LENGTH, &uiLen);

			std::vector<GLchar> vecLogInfo(uiLen);
			glGetShaderInfoLog(uiShader, uiLen, &uiLen, &vecLogInfo[0]);
			LOG_DEV_ERROR(vecLogInfo.data());
			return false;
		}
		return true;
	}
	case SandTable::CompileType::PROGRAM_STATUS:
	{
		GLint iComplied(-1);
		glGetShaderiv(uiShader, GL_LINK_STATUS, &iComplied);
		if (!iComplied)
		{
			GLsizei uiLen(0);
			glGetShaderiv(uiShader, GL_INFO_LOG_LENGTH, &uiLen);

			std::vector<GLchar> vecLogInfo(uiLen);
			glGetShaderInfoLog(uiShader, uiLen, &uiLen, &vecLogInfo[0]);
			LOG_DEV_ERROR(vecLogInfo.data());
			return false;
		}
		return true;
	}
	default:
	{
		SAND_TABLE_ASSERT(false, "Unknow Complie Tye");
		return false;
	}
	}
}
void OpenGLShader::Bind() const
{
	glUseProgram(m_uiRenderID);
}

void OpenGLShader::UnBind() const
{
	glUseProgram(0);
}

void OpenGLShader::SetFloat(const std::string& sName, float fValue)
{
	GLint iLocation = glGetUniformLocation(m_uiRenderID, sName.c_str());
	glUniform1f(iLocation, fValue);
}

void OpenGLShader::SetFloat2(const std::string& sName, const glm::vec2& vec2Value)
{
	GLint iLocation = glGetUniformLocation(m_uiRenderID, sName.c_str());
	glUniform2fv(iLocation, 1, &vec2Value[0]);
}

void OpenGLShader::SetFloat3(const std::string& sName, const glm::vec3& vec3Value)
{
	GLint iLocation = glGetUniformLocation(m_uiRenderID, sName.c_str());
	glUniform3fv(iLocation, 1, &vec3Value[0]);
}

void OpenGLShader::SetFloat4(const std::string& sName, const glm::vec4& vec4Value)
{
	GLint iLocation = glGetUniformLocation(m_uiRenderID, sName.c_str());
	glUniform4fv(iLocation, 1, &vec4Value[0]);
}

void OpenGLShader::SetMat3(const std::string& sName, const glm::mat3& mat3Value)
{
	GLint iLocation = glGetUniformLocation(m_uiRenderID, sName.c_str());
	glUniformMatrix3fv(iLocation, 1, GL_FALSE, &mat3Value[0][0]);
}

void OpenGLShader::SetMat4(const std::string& sName, const glm::mat4& mat4Value)
{
	GLint iLocation = glGetUniformLocation(m_uiRenderID, sName.c_str());
	glUniformMatrix4fv(iLocation, 1, GL_FALSE, &mat4Value[0][0]);
}

void OpenGLShader::SetInt(const std::string& sName, int iValue)
{
	GLint iLocation = glGetUniformLocation(m_uiRenderID, sName.c_str());
	glUniform1i(iLocation, iValue);
}

void OpenGLShader::SetInt2(const std::string& sName, const glm::ivec2& vec2Value)
{
	GLint iLocation = glGetUniformLocation(m_uiRenderID, sName.c_str());
	glUniform2iv(iLocation, 1, &vec2Value[0]);
}

void OpenGLShader::SetInt3(const std::string& sName, const glm::ivec3& vec3Value)
{
	GLint iLocation = glGetUniformLocation(m_uiRenderID, sName.c_str());
	glUniform3iv(iLocation, 1, &vec3Value[0]);
}

void OpenGLShader::SetInt4(const std::string& sName, const glm::ivec4& vec4Value)
{
	GLint iLocation = glGetUniformLocation(m_uiRenderID, sName.c_str());
	glUniform4iv(iLocation, 1, &vec4Value[0]);
}

void OpenGLShader::SetBool(const std::string& sName, bool bValue)
{
	GLint iLocation = glGetUniformLocation(m_uiRenderID, sName.c_str());
	glUniform1i(iLocation, bValue);
}

const std::string& OpenGLShader::GetName() const
{
	return m_sShaderName;
}

SAND_TABLE_NAMESPACE_END