------------------------------------------------------------------------------
-- Graphics API option
------------------------------------------------------------------------------
newoption 
{
    trigger     = "gfxapi",
    value       = "API",
    description = "Choose a graphics API",
    allowed = 
	{
        { "vulkan", "Vulkan graphics API (windows, linux, macosx)" },
        { "dx12", "DirectX 12 (windows)" },
        { "metal", "Metal (macosx)" },
        { "dummy", "No GraphicsAPI, passthrough function calls. (windows, linux, macosx)" },
    }
}

OBSIDIAN_GRAPHICS_API = _OPTIONS["gfxapi"] or "vulkan"
------------------------------------------------------------------------------

------------------------------------------------------------------------------
-- Utils
------------------------------------------------------------------------------
function local_require(path)
	return dofile(path)
end

function this_directory()
    local str = debug.getinfo(2, "S").source:sub(2)
	local path = str:match("(.*/)")
    return path:gsub("\\", "/") -- Replace \\ with /
end
------------------------------------------------------------------------------

------------------------------------------------------------------------------
-- Bug fixes
------------------------------------------------------------------------------
-- Visual Studio: Bugfix for C++ Modules (same module file name per project)
-- https://github.com/premake/premake-core/issues/2177
require("vstudio")
premake.override(premake.vstudio.vc2010.elements, "clCompile", function(base, prj)
    local m = premake.vstudio.vc2010
    local calls = base(prj)

    if premake.project.iscpp(prj) then
		table.insertafter(calls, premake.xmlDeclaration,  function()
			premake.w('<ModuleDependenciesFile>$(IntDir)\\%%(RelativeDir)</ModuleDependenciesFile>')
			premake.w('<ModuleOutputFile>$(IntDir)\\%%(RelativeDir)</ModuleOutputFile>')
			premake.w('<ObjectFileName>$(IntDir)\\%%(RelativeDir)</ObjectFileName>')
		end)
    end

    return calls
end)
------------------------------------------------------------------------------

------------------------------------------------------------------------------
-- Solution
------------------------------------------------------------------------------
MacOSVersion = "14.5"
OutputDir = "%{cfg.buildcfg}-%{cfg.system}"

workspace "Obsidia"
	architecture "x86_64"
	startproject "Editor"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

	flags
	{
		"MultiProcessorCompile"
	}

group "Dependencies"
	include "Vendor/Obsidian/Obsidian/premake5-external"
	include "Vendor/Photon/Photon/premake5-external"
group ""

group "Obsidia"
	include "Obsidia"
group ""

include "Programs/Editor"
include "Programs/Runtime"
------------------------------------------------------------------------------