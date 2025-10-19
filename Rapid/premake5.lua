local Dependencies = local_require("../Dependencies.lua")
local MacOSVersion = MacOSVersion or "14.5"
local OutputDir = OutputDir or "%{cfg.buildcfg}-%{cfg.system}"

project "Rapid"
	kind "StaticLib"
	language "C++"
	cppdialect "C++23"
	staticruntime "On"

	architecture "x86_64"

	warnings "Extra"

	targetdir ("%{wks.location}/bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. OutputDir .. "/%{prj.name}")

	-- Note: VS2022/Make only need the pchheader filename
	pchheader "rppch.h"
	pchsource "Source/Rapid/rppch.cpp"

	files
	{
		"Source/Rapid/**.h",
		"Source/Rapid/**.hpp",
		"Source/Rapid/**.inl",
		"Source/Rapid/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS",
	}

	includedirs
	{
		"Source",
		"Source/Rapid",
	}

	includedirs(Dependencies.Rapid.IncludeDir)
	libdirs(Dependencies.Rapid.LibDir)
	links(remove_from_table(Dependencies.Rapid.LibName, "Rapid"))
	--postbuildcommands(Dependencies.Rapid.PostBuildCommands)
	defines(Dependencies.Rapid.Defines)

	filter "system:windows"
		systemversion "latest"
		staticruntime "on"
		editandcontinue "off"

        defines
        {
            "NOMINMAX"
        }

	filter "system:linux"
		systemversion "latest"
		staticruntime "on"

		-- Note: For some reason gmake, now also needs full pchheader path
		pchheader "Source/Rapid/rppch.h"

		links(Dependencies.Obsidian.LibName)
		links(Dependencies.Photon.LibName)

    filter "system:macosx"
		systemversion(MacOSVersion)
		staticruntime "on"

	filter "action:vs*"
		buildoptions { "/Zc:preprocessor" }

	filter "action:xcode*"
		-- Note: XCode only needs the full pchheader path
		pchheader "Source/Obsidia/rppch.h"

		-- Note: If we don't add the header files to the externalincludedirs
		-- we can't use <angled> brackets to include files.
		externalincludedirs(includedirs())

	filter "configurations:Debug"
		defines "RP_CONFIG_DEBUG"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines "RP_CONFIG_RELEASE"
		runtime "Release"
		optimize "on"
		
	filter "configurations:Distribution"
		defines "RP_CONFIG_DISTRIBUTION"
		runtime "Release"
		optimize "Full"
		linktimeoptimization "on"