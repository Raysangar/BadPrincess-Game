
function loadWave()	


local waveTutorial1= Wave(1,2000);
waveTutorial1:AddWaveNode(WaveNode("Knight",3,"SpawnPoint2"))
AddWave(waveTutorial1)

local waveTutorial2= Wave(2,2000);
waveTutorial2:AddWaveNode(WaveNode("ThiefEnemy",1,"SpawnPoint1"))
AddWave(waveTutorial2)

local waveTutorial3= Wave(3,3000);
waveTutorial3:AddWaveNode(WaveNode("Knight",2,"SpawnPoint1"))
waveTutorial3:AddWaveNode(WaveNode("ThiefEnemy",1,"SpawnPoint1"))
AddWave(waveTutorial3)

local waveTutorial4= Wave(4,3000);
waveTutorial4:AddWaveNode(WaveNode("Knight",2,"SpawnPoint2"))
waveTutorial4:AddWaveNode(WaveNode("ThiefEnemy",1,"SpawnPoint2"))
AddWave(waveTutorial4)

local waveTutorial5= Wave(5,3000);
waveTutorial5:AddWaveNode(WaveNode("Knight",2,"SpawnPoint1"))
AddWave(waveTutorial5)

local waveTutorial6= Wave(6,3000);
waveTutorial6:AddWaveNode(WaveNode("Knight",2,"SpawnPoint2"))
AddWave(waveTutorial6)

local waveTutorial7 = Wave(7,3000);
waveTutorial7:AddWaveNode(WaveNode("Knight",4,"SpawnPoint2"))
AddWave(waveTutorial7)

local waveTutorial8= Wave(8,3000);
waveTutorial8:AddWaveNode(WaveNode("Knight",4,"SpawnPoint1"))
AddWave(waveTutorial8)

local waveTutorial9 = Wave(9,3000);
waveTutorial9:AddWaveNode(WaveNode("Knight",2,"SpawnPoint2"))
AddWave(waveTutorial9)

local waveTutorial10 = Wave(10,3000);
waveTutorial10:AddWaveNode(WaveNode("Knight",1,"SpawnPoint1"))
waveTutorial10:AddWaveNode(WaveNode("Assassin",1,"SpawnPoint1"))
AddWave(waveTutorial10)
--[[
-- pureba campamiento
local waveTutorial11= Wave(11,0);
waveTutorial11:AddWaveNode(WaveNode("Knight",1,"Camp1"))
AddWave(waveTutorial11)

local waveTutorial12= Wave(12,0);
waveTutorial12:AddWaveNode(WaveNode("Knight",10,"Camp1"))
waveTutorial12:AddWaveNode(WaveNode("Assassin",2,"Camp1"))
AddWave(waveTutorial12)
]]--
end