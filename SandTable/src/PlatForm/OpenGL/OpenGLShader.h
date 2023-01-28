#pragma once
#include "SandTable/Render/Shader.h"

SAND_TABLE_NAMESPACE_BEGIN

class OpenGLShader :public Shader
{
public:
	OpenGLShader(const std::vector<ShaderInfo>& vecShaderInfo, const std::string& sShaderName);
	virtual ~OpenGLShader();
private:
	bool CheckCompileError(unsigned int uiShader, CompileType type);

	// Í¨¹ý Shader ¼Ì³Ð
	virtual void Bind() const override;
	virtual void UnBind() const override;
	virtual void SetFloat(const std::string& sName, float fValue) override;
	virtual void SetFloat2(const std::string& sName, const glm::vec2& vec2Value) override;
	virtual void SetFloat3(const std::string& sName, const glm::vec3& vec3Value) override;
	virtual void SetFloat4(const std::string& sName, const glm::vec4& vec4Value) override;
	virtual void SetMat3(const std::string& name, const glm::mat3& mat3Value) override;
	virtual void SetMat4(const std::string& sName, const glm::mat4& mat4Value) override;
	virtual void SetInt(const std::string& sName, int iValue) override;
	virtual void SetInt2(const std::string& sName, const glm::ivec2& vec2Value) override;
	virtual void SetInt3(const std::string& sName, const glm::ivec3& vec3Value) override;
	virtual void SetInt4(const std::string& sName, const glm::ivec4& vec4Value) override;
	virtual void SetIntArray(const std::string& sName, const int* pValues,unsigned int uiCount) override;
	virtual void SetBool(const std::string& sName, bool bValue) override;
	virtual void SetUniformBlock(const std::string& sName, int iBlockBinding) override;
	virtual const std::string& GetName() const override;
private:
	unsigned int m_uiRenderID;
	std::string m_sShaderName;
};

SAND_TABLE_NAMESPACE_END