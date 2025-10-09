local Dependencies = local_require("../../Dependencies.lua")
local MacOSVersion = MacOSVersion or "14.5"
local OutputDir = OutputDir or "%{cfg.buildcfg}-%{cfg.system}"

project "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++23"
	staticruntime "On"

	architecture "x86_64"

	warnings "Extra"

	targetdir ("%{wks.location}/bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. OutputDir .. "/%{prj.name}")

	files
	{
		"Source/**.h",
		"Source/**.hpp",
		"Source/**.inl",
		"Source/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS",
	}

	includedirs
	{
		"Source",
	}

	includedirs(Dependencies.Obsidia.IncludeDir) -- Note: Includes Source/Obsidia
	libdirs(Dependencies.Obsidia.LibDir)
	postbuildcommands(Dependencies.Obsidia.PostBuildCommands)

	filter "system:windows"
		systemversion "latest"
		staticruntime "on"
		editandcontinue "off"

		links("Obsidia")

        defines
        {
            "NOMINMAX"
        }

	filter "system:linux"
		systemversion "latest"
		staticruntime "on"

		-- Note: On linux, it needs to relink all dependencies
		links(Dependencies.Obsidia.LibName)

    filter "system:macosx"
		systemversion(MacOSVersion)
		staticruntime "on"

		links("Obsidia")

	filter "action:vs*"
		buildoptions { "/Zc:preprocessor" }

	filter "action:xcode*"
		-- Note: If we don't add the header files to the externalincludedirs
		-- we can't use <angled> brackets to include files.
		externalincludedirs(includedirs())

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		runtime "Release"
		optimize "Full"
		linktimeoptimization "on"