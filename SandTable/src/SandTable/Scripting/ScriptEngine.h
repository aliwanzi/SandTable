#pragma once

SAND_TABLE_NAMESPACE_BEGIN

class ScriptEngine
{
public:
	static void Init();
	static void ShutDown();
private:
	static void InitMono();
};

SAND_TABLE_NAMESPACE_END

