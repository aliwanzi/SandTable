include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "SandTable"
	architecture "x86_64"
	startproject "SandBox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
	include "SandTable/vendor/GLFW"
	include "SandTable/vendor/GL3W"
	include "SandTable/vendor/imgui"
	include "SandTable/vendor/yaml-cpp"
	include "SandTable/vendor/Box2D"
group ""


include "SandTable"
include "SandBox"
