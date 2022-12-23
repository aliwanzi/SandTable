#pragma once

#include <memory>

#ifdef SAND_TABLE_ENABLE_ASSERTS
	#define SAND_TABLE_ASSERT(x,...) {if(!(x)) {LOG_DEV_ERROR("Assertion Failed: {0}",__VA_ARGS__); __debugbreak();}}

#endif // SAND_TABLE_ENABLE_ASSERTS

#define BIT(x) (1<<x)

#define BIND_EVENT_FUN(x) std::bind(&x, this, std::placeholders::_1)

#define SAND_TABLE_PROFILE 1

#ifndef SAND_TABLE_NAMESPACE_BEGIN
#define SAND_TABLE_NAMESPACE_BEGIN \
	namespace SandTable {
#endif // !SAND_TABLE_NAMESPACE_BEGIN

#ifndef SAND_TABLE_NAMESPACE_END
#define SAND_TABLE_NAMESPACE_END \
	}
#endif // !SAND_TABLE_NAMESPACE_END


SAND_TABLE_NAMESPACE_BEGIN
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
SAND_TABLE_NAMESPACE_END