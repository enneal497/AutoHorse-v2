function replace_in_file(filepath, plugin_name)
    if not os.exists(filepath) then
        return false, "File not found: " .. filepath
    end

    local content = io.readfile(filepath)
    if not content then
        return false, "Failed to read file: " .. filepath
    end

    local replaced = content:gsub("__plugin_name__", plugin_name)
    if replaced ~= content then
        io.writefile(filepath, replaced)
        return true
    else
        return false, "No replacement made in: " .. filepath
    end
end

function main(plugin_name)
    if not plugin_name then
        print("Usage: xmake lua setup.lua <plugin_name>")
        return
    end

    local config_old = path.join("config", "__plugin_name__.ini")
    local config_new = path.join("config", plugin_name .. ".ini")

    if os.exists(config_old) then
        os.mv(config_old, config_new)
        print("Renamed config file to: " .. config_new)
    else
        print("Note: Config file not found: " .. config_old)
    end

    -- List of files to process
    local files_to_update = {
        "xmake.lua",
        path.join("src", "Settings.cpp"),
    }

    for _, file in ipairs(files_to_update) do
        local ok, msg = replace_in_file(file, plugin_name)
        if ok then
            print("Updated: " .. file)
        else
            print("Skipped: " .. msg)
        end
    end
end