
function loadWave()	

local waveCamp = Wave(1,0)
waveCamp:AddWaveNode(WaveNode("Knight",3,"Camp"))
AddWave(waveCamp)

local finalWave1 = Wave(2,2000)
finalWave1:AddWaveNode(WaveNode("Knight",3,"SpawnPoint1"))
AddWave(finalWave1)

local finalWave2 = Wave(3,2000)
finalWave2:AddWaveNode(WaveNode("Knight",3,"SpawnPoint1"))
AddWave(finalWave2)

end

--insertar el id de la ultima oleada asi que si se crean oleada dinamica no se soverponen con la estatica 
idWave = 3
