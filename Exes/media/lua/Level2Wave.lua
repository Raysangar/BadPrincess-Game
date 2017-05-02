
function loadWave()	

local waveCamp = Wave(1,0)
waveCamp:AddWaveNode(WaveNode("Knight",2,"Camp"))
waveCamp:AddWaveNode(WaveNode("Assassin",1,"Camp"))
AddWave(waveCamp)

local finalWave1 = Wave(2,2000)
finalWave1:AddWaveNode(WaveNode("ThiefEnemy",1,"SpawnPoint1"))
finalWave1:AddWaveNode(WaveNode("Knight",3,"SpawnPoint1"))
finalWave1:AddWaveNode(WaveNode("Assassin",2,"SpawnPoint1"))

AddWave(finalWave1)

local finalWave2 = Wave(3,2000)
finalWave1:AddWaveNode(WaveNode("ThiefEnemy",1,"SpawnPoint1"))
finalWave1:AddWaveNode(WaveNode("Knight",4,"SpawnPoint1"))
finalWave1:AddWaveNode(WaveNode("Assassin",2,"SpawnPoint1"))
AddWave(finalWave2)

local waveCamp2 = Wave(4,0)
waveCamp2:AddWaveNode(WaveNode("Knight",3,"Camp2"))
waveCamp2:AddWaveNode(WaveNode("Assassin",3,"Camp2"))
AddWave(waveCamp2)

end

--insertar el id de la ultima oleada asi que si se crean oleada dinamica no se soverponen con la estatica 
idWave = 4
