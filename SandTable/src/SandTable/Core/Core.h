#pragma once

#include <memory>

#ifdef  SAND_TABLE_PLATFORM_WINDOWS
#if SAND_TABLE_DYNAMIC_LINK
	#ifdef SAND_TABLE_BUILD_DLL
		#define SAND_TABLE_API _declspec(dllexport)
	#else
		#define SAND_TABLE_API _declspec(dllimport)
	#endif // SAND_TABLE_BUILD_DLL
#else
	#define SAND_TABLE_API
#endif
#else
	#error SandTabel only support Windows!
#endif //  SAND_TABLE_PLATFORM_WINDOWS

#ifdef SAND_TABLE_ENABLE_ASSERTS
	#define SAND_TABLE_ASSERT(x,...) {if(!(x)) {LOG_DEV_ERROR("Assertion Failed: {0}",__VA_ARGS__); __debugbreak();}}

#endif // SAND_TABLE_ENABLE_ASSERTS

#define BIT(x) (1<<x)

#define BIND_EVENT_FUN(x) std::bind(&x, this, std::placeholders::_1)