#include "pch.h"
#include "SandTable/Utils/PlatformUtils.h"
#include "SandTable/Core/Application.h"
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>


SAND_TABLE_NAMESPACE_BEGIN

std::string PlatformUtils::OpenFile(const char* filter)
{
	OPENFILENAMEA ofn;
	CHAR szFile[UCHAR_MAX] = { 0 };
	CHAR currentDir[UCHAR_MAX] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = glfwGetWin32Window(static_cast<GLFWwindow*>(Application::GetApplication()->GetWindow()->GetNativeWindow()));
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	if (GetCurrentDirectoryA(UCHAR_MAX, currentDir))
		ofn.lpstrInitialDir = currentDir;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&ofn) == TRUE)
		return ofn.lpstrFile;

	return std::string();

}

std::string PlatformUtils::SaveFile(const char* filter)
{
	OPENFILENAMEA ofn;
	CHAR szFile[UCHAR_MAX] = { 0 };
	CHAR currentDir[UCHAR_MAX] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = glfwGetWin32Window(static_cast<GLFWwindow*>(Application::GetApplication()->GetWindow()->GetNativeWindow()));
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	if (GetCurrentDirectoryA(UCHAR_MAX, currentDir))
		ofn.lpstrInitialDir = currentDir;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	// Sets the default extension by extracting it from the filter
	ofn.lpstrDefExt = strchr(filter, '\0') + 1;

	if (GetSaveFileNameA(&ofn) == TRUE)
		return ofn.lpstrFile;

	return std::string();
}

SAND_TABLE_NAMESPACE_END


