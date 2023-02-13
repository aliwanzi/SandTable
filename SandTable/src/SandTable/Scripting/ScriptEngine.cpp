#include "pch.h"
#include "ScriptEngine.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

SAND_TABLE_NAMESPACE_BEGIN

void ScriptEngine::Init()
{

}

void ScriptEngine::ShutDown()
{

}

void ScriptEngine::InitMono()
{
	mono_set_assemblies_path("mono/lib");

	auto pRootDomain = mono_jit_init("SandTableJITRuntime");
	SAND_TABLE_ASSERT(pRootDomain, "pRootDomain is null in InitMono");
}

SAND_TABLE_NAMESPACE_END