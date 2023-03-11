#pragma once

SAND_TABLE_NAMESPACE_BEGIN

class FileSystem
{
public:
	static std::string OpenFile(const char* filter);
	static std::string SaveFile(const char* filter);

	static Ref<DataBuffer> ReadFileDataBuffer(const std::filesystem::path& sFilePath);
};

SAND_TABLE_NAMESPACE_END

