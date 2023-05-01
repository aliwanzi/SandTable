project "SandTable"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "src/pch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",

		"vendor/ImGuizmo/ImGuizmo.h",
		"vendor/ImGuizmo/ImGuizmo.cpp"
	}

	includedirs
	{
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GL3W}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.mono}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.filewatch}",
		"%{IncludeDir.OpenMesh}"
	}

	links
	{
		"GLFW",
		"GL3W",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib",
		"Box2D",

		"%{Library.mono}",
		"%{Library.OpenMeshCore}",
		"%{Library.OpenMeshTools}",
	}

	filter "files:vendor/ImGuizmo/**.cpp"
	flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"SAND_TABLE_ENABLE_ASSERTS",
			"SAND_TABLE_PLATFORM_WINDOWS",
			"SAND_TABLE_BUILD_DLL",
			"SAND_TABLE_RENDER_OPENGL",
			"_USE_MATH_DEFINES"
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