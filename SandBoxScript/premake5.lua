local SandTableRootDir = '../'
include (SandTableRootDir .. "/vendor/premake/premake_customization/solution_items.lua")

workspace "SandBoxScript"
	architecture "x86_64"
	startproject "SandBoxScript"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "SandBoxScript"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("../SandBox/script/")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"src/**.cs"
	}

	links
	{
		"SandTableScript"
	}

	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"


include (SandTableRootDir .. "/SandTableScript")
