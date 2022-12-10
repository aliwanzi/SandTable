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

include "SandTable/vendor/GLFW"
include "SandTable/vendor/GL3W"

project "SandTable"
	location "SandTable"
	kind "SharedLib"
	language "C++"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "%{prj.name}/src/pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs 
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GL3W}"
	}

	links
	{
		"GLFW",
		"GL3W",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"SAND_TABLE_ENABLE_ASSERTS",
			"SAND_TABLE_PLATFORM_WINDOWS",
			"SAND_TABLE_BUILD_DLL"
		}
		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/SandBox")
		}
	
	filter "configurations:Degug"
		defines "SAND_TABLE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "SAND_TABLE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "SAND_TABLE_DIST"
		optimize "On"


project "SandBox"
	location "SandBox"
	kind "ConsoleApp"
	language "C++"

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
		"SandTable/src"
	}

	links
	{
		"SandTable"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		defines
		{
			"SAND_TABLE_PLATFORM_WINDOWS"
		}

	filter "configurations:Degug"
		defines "SAND_TABLE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "SAND_TABLE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "SAND_TABLE_DIST"
		optimize "On"