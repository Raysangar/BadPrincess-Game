
function loadWave()	


local wave1= Wave(1,"Time","2000",500);
wave1:AddWaveNode(WaveNode("ThiefEnemy",5,"SpawnPoint1"))
AddWave(wave1)
--[[
local wave12= Wave(2,"Message","AimsCompleted1",500);
wave12:AddWaveNode(WaveNode("Dummy",3,"SpawnPoint2"))
AddWave(wave12)
--]]
local waveTutorial1= Wave(3,1000);
waveTutorial1:AddWaveNode(WaveNode("Knight",2,"SpawnPoint1"))
AddWave(waveTutorial1)

local waveTutorial2= Wave(4,1000);
waveTutorial2:AddWaveNode(WaveNode("Knight",4,"SpawnPoint2"))
AddWave(waveTutorial2)

local waveTutorial3= Wave(5,1000);
waveTutorial3:AddWaveNode(WaveNode("ThiefEnemy",1,"SpawnPoint1"))
waveTutorial3:AddWaveNode(WaveNode("ThiefEnemy",1,"SpawnPoint2"))
AddWave(waveTutorial3)

local waveTutorial4= Wave(6,1000);
waveTutorial4:AddWaveNode(WaveNode("Knight",3,"SpawnPoint1"))
waveTutorial4:AddWaveNode(WaveNode("ThiefEnemy",2,"SpawnPoint2"))
waveTutorial4:AddWaveNode(WaveNode("Knight",1,"SpawnPoint2"))
waveTutorial4:AddWaveNode(WaveNode("ThiefEnemy",2,"SpawnPoint1"))
AddWave(waveTutorial4)

end