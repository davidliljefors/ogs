workspace "ogs"
	architecture "x64"
	startproject "ogs"

	configurations
	{
		"Debug",
		"Release",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "ogs/vendor/GLFW/include"
IncludeDir["Glad"] = "ogs/vendor/Glad/include"
IncludeDir["ImGui"] = "ogs/vendor/imgui"
IncludeDir["glm"] = "ogs/vendor/glm"
IncludeDir["stb_image"] = "ogs/vendor/stb_image"
IncludeDir["fmt"] = "ogs/vendor/fmt/include"

include "ogs/vendor/GLFW"
include "ogs/vendor/Glad"
include "ogs/vendor/fmt"
include "ogs/vendor/imgui"

project "ogs"
	location "ogs"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	warnings "Extra"
	flags  { "FatalWarnings" }

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.fmt}",
		"%{IncludeDir.ImGui}",
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"fmt",
		"ImGui",
		"opengl32.lib",
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"OGS_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE",
			"GLM_FORCE_SILENT_WARNINGS"
		}

	filter "configurations:Debug"
		defines "OGS_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "OGS_RELEASE"
		runtime "Release"
		optimize "on"