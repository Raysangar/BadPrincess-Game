
function loadWave()	

local camp1 = Wave(4, 0)
camp1:AddWaveNode(WaveNode("Knight",4,"Camp"))
camp1:AddWaveNode(WaveNode("Assassin",1,"Camp"))
AddWave(camp1)

local camp2 = Wave(5, 0)
camp2:AddWaveNode(WaveNode("Knight",4,"Camp2"))
camp2:AddWaveNode(WaveNode("Assassin",1,"Camp2"))
AddWave(camp2)

local camp3 = Wave(6, 0)
camp3:AddWaveNode(WaveNode("Knight",4,"Camp3"))
camp3:AddWaveNode(WaveNode("Assassin",1,"Camp3"))
AddWave(camp3)

local camp4 = Wave(7, 0)
camp4:AddWaveNode(WaveNode("Knight",4,"Camp4"))
camp4:AddWaveNode(WaveNode("Assassin",1,"Camp4"))
AddWave(camp4)

local camp5 = Wave(8, 0)
camp5:AddWaveNode(WaveNode("Knight",4,"Camp5"))
camp5:AddWaveNode(WaveNode("Assassin",1,"Camp5"))
AddWave(camp5)

local camp6 = Wave(9, 0)
camp6:AddWaveNode(WaveNode("Knight",4,"Camp6"))
camp6:AddWaveNode(WaveNode("Assassin",1,"Camp3"))
AddWave(camp6)

local camp7 = Wave(10, 0)
camp7:AddWaveNode(WaveNode("Knight",4,"Camp7"))
camp7:AddWaveNode(WaveNode("Assassin",1,"Camp4"))
AddWave(camp7)

local camp8 = Wave(11, 0)
camp8:AddWaveNode(WaveNode("Knight",4,"Camp8"))
camp8:AddWaveNode(WaveNode("Assassin",1,"Camp5"))
AddWave(camp8)

local camp9 = Wave(12, 0)
camp9:AddWaveNode(WaveNode("Knight",4,"Camp9"))
camp9:AddWaveNode(WaveNode("Assassin",1,"Camp5"))
AddWave(camp9)

end

--insertar el id de la ultima oleada asi que si se crean oleada dinamica no se soverponen con la estatica 
idWave = 12
