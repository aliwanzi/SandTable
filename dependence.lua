
-- SandTable Dependencies
IncludeDir = {}
IncludeDir["stb_image"] = "%{wks.location}/SandTable/vendor/stb_image"
IncludeDir["yaml_cpp"] = "%{wks.location}/SandTable/vendor/yaml-cpp/include"
IncludeDir["GLFW"] = "%{wks.location}/SandTable/vendor/GLFW/include"
IncludeDir["GL3W"] = "%{wks.location}/SandTable/vendor/GL3W/include"
IncludeDir["ImGui"] = "%{wks.location}/SandTable/vendor/ImGui"
IncludeDir["ImGuizmo"] = "%{wks.location}/SandTable/vendor/ImGuizmo"
IncludeDir["glm"] = "%{wks.location}/SandTable/vendor/glm"
IncludeDir["entt"] = "%{wks.location}/SandTable/vendor/entt/include"
IncludeDir["Box2D"] = "%{wks.location}/SandTable/vendor/Box2D/include"
IncludeDir["mono"] = "%{wks.location}/SandTable/vendor/mono/include"
IncludeDir["filewatch"] = "%{wks.location}/SandTable/vendor/filewatch/"
IncludeDir["OpenMesh"] = "%{wks.location}/SandTable/vendor/OpenMesh/include"

LibraryDir = {}
LibraryDir["mono"] = "%{wks.location}/SandTable/vendor/mono/lib/%{cfg.buildcfg}"
LibraryDir["OpenMesh"] = "%{wks.location}/SandTable/vendor/OpenMesh/lib/%{cfg.buildcfg}"

Library = {}
Library["mono"] = "%{LibraryDir.mono}/mono-2.0-sgen.lib"
Library["OpenMeshCore"] = "%{LibraryDir.OpenMesh}/OpenMeshCore.lib"
Library["OpenMeshTools"] = "%{LibraryDir.OpenMesh}/OpenMeshTools.lib"

BinaryDir = {}
BinaryDir["mono"] = "%{wks.location}/SandTable/vendor/mono/bin/%{cfg.buildcfg}"
BinaryDir["OpenMesh"] = "%{wks.location}/SandTable/vendor/OpenMesh/bin/%{cfg.buildcfg}"

Binary = {}
Binary["mono"] = "%{BinaryDir.mono}/mono-2.0-sgen.dll"
Binary["OpenMeshCore"] = "%{BinaryDir.OpenMesh}/OpenMeshCore.dll"
Binary["OpenMeshTools"] = "%{BinaryDir.OpenMesh}/OpenMeshTools.dll"
