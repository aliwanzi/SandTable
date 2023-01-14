#pragma once

SAND_TABLE_NAMESPACE_BEGIN

class PlatformUtils
{
public:
	static std::string OpenFile(const char* filter);
	static std::string SaveFile(const char* filter);
};

SAND_TABLE_NAMESPACE_END

