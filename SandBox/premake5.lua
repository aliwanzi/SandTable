project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/SandTable/vendor/spdlog/include",
		"%{wks.location}/SandTable/src",
		"%{wks.location}/SandTable/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.Box2D}"
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

	filter "configurations:Debug"
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
