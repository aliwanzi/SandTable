#pragma once

SAND_TABLE_NAMESPACE_BEGIN

class ScriptEngine
{
public:
	static void Init();
	static void LoadAssembly(const std::filesystem::path& sAssemblyPath);
private:
	static void InitMono();
	static void InvokeClass(const std::string& sClassNameSpace, const std::string& sClassName);
};

SAND_TABLE_NAMESPACE_END

