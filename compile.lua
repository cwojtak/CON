function dec_hex(val)
    local b, k, out, i, d = 16, "0123456789ABCDEF", "", 0
    while val > 0 do
        i = i + 1
        val, d = math.floor(val/b),math.mod(val, b) + 1
        out = string.sub(k, d, d) .. out
    end
    return out
end

function compile(filename)
	local test = io.open(filename)
	if not test then
		throwError("FileNotFoundException", "Unable to load file: " .. filename .. "!")
		os.exit(1)
	end
	
	local line_num = 1
	for line in io.lines(filename)
		if line == nil then break end --Checks to make sure line exists.
		
		try = pcall(var_construct(line)) 
		
		if try == false then
			throwError("VariableSyntaxError", "Unable to parse line " .. line_num .. ".")
		end
		
		line_num = line_num + 1
	end
end

function var_construct(line)
	local list = {}
	local i = 0
	while i ~= line:len() do --Inserts all the characters of the line into a list.
		table.insert(list, line:sub(i, i))
		i = i + 1
	end
	
	local shouldDelete = true
	for i, character in ipairs(list) do
		if character == " " and shouldDelete == true then
			table.remove(list, i)
		end
		if not character == " " then
			shouldDelete = false
		end
	end
	
	local varname_char = {}
	if list[1] .. list[2] .. list[3] == "var" then --Based on the assumption list items are updated after deletion.
		if list[4] == " " and string.find(line, " equals") then
			for i, character in ipairs(list) do
				if i > 4 then
					if list[i] .. list[i + 1] .. list[i + 2] .. list[i + 3] .. list[i + 4] .. list[i + 5] .. list[i + 6] ~= " equals" then
						table.insert(varname_char, character)
					end
				end
			end
		else 
			throwError("VariableSyntaxError", "Unable to parse line " .. line_num .. ".")
		end
	else return end
	end
	
	local varname = "" --TODO: Add variable name from varname_char.
end


function throwError(err, msg)
	print("A fatal exception has occured! Compilation can not continue!")
	print("Exception Type: " .. err .. ".")
	print(msg)
	error()
end