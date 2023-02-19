#pragma once

SAND_TABLE_NAMESPACE_BEGIN
class ScriptClass;
class ScriptEngine
{
public:
	static void Init();
	static void LoadAssembly(const std::filesystem::path& sAssemblyPath);
	static const std::unordered_map<std::string, Ref<ScriptClass>>& GetEntityClass();
private:
	static void InitMono();
	static void InvokeClass(const std::string& sClassNameSpace, const std::string& sClassName);
	static void LoadAssemblyClass();
};

SAND_TABLE_NAMESPACE_END

