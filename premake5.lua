workspace "SandTable"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "SandTable/vendor/GLFW/include"
IncludeDir["GL3W"] = "SandTable/vendor/GL3W/include"
IncludeDir["ImGui"] = "SandTable/vendor/imgui"
IncludeDir["glm"] = "SandTable/vendor/glm"
IncludeDir["stb_image"] = "SandTable/vendor/stb_image"

include "SandTable/vendor/GLFW"
include "SandTable/vendor/GL3W"
include "SandTable/vendor/imgui"

project "SandTable"
	location "SandTable"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "%{prj.name}/src/pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp"
	}
	
	includedirs 
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GL3W}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"GL3W",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"SAND_TABLE_ENABLE_ASSERTS",
			"SAND_TABLE_PLATFORM_WINDOWS",
			"SAND_TABLE_BUILD_DLL",
			"SAND_TABLE_RENDER_OPENGL"
		}

	filter "configurations:Degug"
		defines "SAND_TABLE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SAND_TABLE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SAND_TABLE_DIST"
		runtime "Release"
		optimize "on"


project "SandBox"
	location "SandBox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"SandTable/vendor/spdlog/include",
		"SandTable/src",
		"SandTable/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"SandTable"
	}

	filter "system:windows"

		systemversion "latest"
		defines
		{
			"SAND_TABLE_PLATFORM_WINDOWS",
			"SAND_TABLE_ENABLE_ASSERTS"
		}

	filter "configurations:Degug"
		defines "SAND_TABLE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SAND_TABLE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SAND_TABLE_DIST"
		runtime "Release"
		optimize "on"