#include "pch.h"
#include "ScriptClass.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"

SAND_TABLE_NAMESPACE_BEGIN

ScriptClass::ScriptClass(const std::string& sClassNameSpace, const std::string& sClassName, 
	const Ref<MonoDomain>& spAppDomain,const Ref<MonoImage>& spCoreMonoImage)
{
	m_pMonoClass = mono_class_from_name(spCoreMonoImage.get(), sClassNameSpace.c_str(), sClassName.c_str());
	m_pMonoObject = mono_object_new(spAppDomain.get(), m_pMonoClass);
	mono_runtime_object_init(m_pMonoObject);
}

MonoMethod* ScriptClass::GetMonoMethod(const std::string& sMethodName, unsigned int uiParaCount)
{
	return mono_class_get_method_from_name(m_pMonoClass, sMethodName.c_str(), uiParaCount);
}

void ScriptClass::InvokeMethod(MonoMethod* pMonoMethod, void** pParams)
{
	mono_runtime_invoke(pMonoMethod, m_pMonoObject, pParams, nullptr);
}
SAND_TABLE_NAMESPACE_END
