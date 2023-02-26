local SandTableRootDir = '../'
include (SandTableRootDir .. "/vendor/premake/premake_customization/solution_items.lua")

workspace "SandBoxScript"
	architecture "x86_64"
	startproject "SandBox"

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

	targetdir ("Binaries")
	objdir ("Intermediates")

	files 
	{
		"Source/**.cs",
		"Properties/**.cs"
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

group "SandTable"
	include (SandTableRootDir .. "/SandTableScript")
group ""
