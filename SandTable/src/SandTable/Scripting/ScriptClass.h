#pragma once

extern "C" {
	typedef struct	_MonoClass		MonoClass;
	typedef struct	_MonoObject		MonoObject;
	typedef struct	_MonoMethod		MonoMethod;
	typedef struct	_MonoAssembly	MonoAssembly;
	typedef struct	_MonoDomain		MonoDomain;
	typedef struct	_MonoImage		MonoImage;
	typedef struct	_MonoClassField	MonoClassField;
}

SAND_TABLE_NAMESPACE_BEGIN
class ScriptClass
{
public:
	ScriptClass() = default;
	ScriptClass(const std::string& sClassNameSpace, const std::string& sClassName,
		const Ref<MonoDomain>& spAppDomain, const Ref<MonoImage>& spCoreMonoImage);

	MonoMethod* GetMonoMethod(const std::string& sMethodName, unsigned int uiParaCount);
	void InvokeMethod(MonoMethod* pMonoMethod, void** params = nullptr);
private:
	MonoClass* m_pMonoClass;
	MonoObject* m_pMonoObject;
};
SAND_TABLE_NAMESPACE_END

