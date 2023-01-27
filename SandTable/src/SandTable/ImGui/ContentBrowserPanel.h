#pragma once
#include <filesystem>
SAND_TABLE_NAMESPACE_BEGIN

class Texture;
class ContentBrowserPanel
{
public:
	ContentBrowserPanel();
	void OnImGuiRender();

private:
	std::filesystem::path m_currentDirectory;
	Ref<Texture> m_spDirectoryIcon;
	Ref<Texture> m_spFileIcon;
};

SAND_TABLE_NAMESPACE_END

