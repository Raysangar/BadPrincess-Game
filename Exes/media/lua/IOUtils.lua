function serializeTable(val, name, skipnewlines, depth)
    skipnewlines = skipnewlines or false
    depth = depth or 0

    local tmp = StringDuplicate(" ", depth)

    if name then tmp = tmp .. name .. " = " end

    if type(val) == "table" then
        tmp = tmp .. "{" .. (not skipnewlines and "\n" or "")

        for k, v in pairs(val) do
            tmp =  tmp .. serializeTable(v, k, skipnewlines, depth + 1) .. "," .. (not skipnewlines and "\n" or "")
        end

        tmp = tmp .. StringDuplicate(" ", depth) .. "}"
    elseif type(val) == "number" then
        tmp = tmp .. val
    elseif type(val) == "string" then
        tmp = tmp .. val
    elseif type(val) == "boolean" then
        tmp = tmp .. (val and "true" or "false")
    else
        tmp = tmp .. "\"[inserializeable datatype:" .. type(val) .. "]\""
    end

    return tmp
end

function saveUserProfile()
	local tableStringfy = serializeTable(UserProfile,"UserProfile")
	SaveStringToFile("media/lua/userProfile.lua",tableStringfy)
end