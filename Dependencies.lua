------------------------------------------------------------------------------
-- Utils
------------------------------------------------------------------------------
local function local_require(path)
	return dofile(path)
end

local function this_directory()
    local str = debug.getinfo(2, "S").source:sub(2)
	local path = str:match("(.*/)")
    return path:gsub("\\", "/") -- Replace \\ with /
end

function append_to_table(dest, value)
	if type(value) == "table" then
		for _, v in ipairs(value) do
        	table.insert(dest, v)
    	end
    else
		table.insert(dest, value)
    end

	return dest
end

function remove_from_table(dest, filter)
    for i = #dest, 1, -1 do  -- Iterate backwards
        local value = dest[i]

		-- Note: Allows lua patterns
        if string.find(value, filter) ~= nil then
            table.remove(dest, i)
        end
    end

	return dest
end

function copy_table(tbl)
    if type(tbl) ~= "table" then 
		return tbl 
	end

    local copy = {}
    for k, v in pairs(tbl) do
        copy[k] = copy_table(v)
    end
	
    return copy
end
------------------------------------------------------------------------------

------------------------------------------------------------------------------
-- Graphics API
------------------------------------------------------------------------------
OBSIDIAN_GRAPHICS_API = OBSIDIAN_GRAPHICS_API or "vulkan" 
------------------------------------------------------------------------------

------------------------------------------------------------------------------
-- Dependencies
------------------------------------------------------------------------------
local Dependencies =
{
    -- External dependencies
    Nano = 
    {
        IncludeDir = "Vendor/Nano/Nano/Nano/include"
    },

	Obsidian = local_require("Vendor/Obsidian/Obsidian/Dependencies.lua").Obsidian,
	Photon = local_require("Vendor/Photon/Photon/Dependencies.lua").Photon,

    -- Internal dependencies
    Obsidia = 
    {
		IncludeDir = {},
        LibDir = {},
        LibName = {},
        PostBuildCommands = {}
    }
}
------------------------------------------------------------------------------

------------------------------------------------------------------------------
-- Append Dependencies
------------------------------------------------------------------------------
-- IncludeDirs
append_to_table(Dependencies.Obsidia.IncludeDir, this_directory() .. "Obsidia/Source/")
append_to_table(Dependencies.Obsidia.IncludeDir, Dependencies.Nano.IncludeDir)
append_to_table(Dependencies.Obsidia.IncludeDir, Dependencies.Obsidian.IncludeDir)
append_to_table(Dependencies.Obsidia.IncludeDir, Dependencies.Photon.IncludeDir)

-- LibDir
append_to_table(Dependencies.Obsidia.LibDir, Dependencies.Obsidian.LibDir)
append_to_table(Dependencies.Obsidia.LibDir, Dependencies.Photon.LibDir)

-- LibNames
append_to_table(Dependencies.Obsidia.LibName, "Obsidia")
append_to_table(Dependencies.Obsidia.LibName, Dependencies.Obsidian.LibName)
append_to_table(Dependencies.Obsidia.LibName, Dependencies.Photon.LibName)

-- PostBuildCommands
append_to_table(Dependencies.Obsidia.PostBuildCommands, Dependencies.Obsidian.PostBuildCommands)
append_to_table(Dependencies.Obsidia.PostBuildCommands, Dependencies.Photon.PostBuildCommands)
------------------------------------------------------------------------------

return Dependencies