-- ----
-- Cargador de mapas. Llama a las funciones correspondientes
-- del motor.
-- NO debe llamarse desde lua para cargar un nuevo mapa,
-- pues el motor no estará preparado para recibir las
-- solicitudes de creacion de entidades nuevas!!!
-- ----

function loadMap(mapFile)	
	local file = loadfile(mapFile)

	if file == nil then
		return 0
	end

	-- Ejecutamos
	Map = nil
	file()
	
	if Map == nil then
		print("MAP == nil")
	   return 0
	end

	local toString = function (v)
				if type(v) == "boolean" then
					if v then return "true"
					else return "false" end
				end
				if type(v) == "table" then
					return v[1].." "..v[2].." "..v[3]
				end
				return ""..v
			end

	for key, value in pairs(Map) do
	
		local entity = MapEntity(key)
		entity:SetType(value.type)
		for k, v in pairs(value) do
			if k ~= "type" then
				entity:SetAttrib(k, toString(v))
			end
		end
		AddEntity(entity)
	end

	return 1

end