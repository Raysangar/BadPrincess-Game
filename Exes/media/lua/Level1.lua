function loadGameAction()
	introduction()
	summonWarriors()
	attackCamp()
	defendPrincess()

	local m = GameOver(true)
	local b = SendMessageAction("GameManager",m)
	AddActionNode(b)

end

function introduction()

	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))
	
	AddActionNode(MoveCameraAction("Tent2"))
	AddActionNode(WaitTimeAction(5000))
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(ShowMessageAction("A","Your grace! Several intruders have decided to camp in the west of here!   (Push SPACE to continue...)",0.55,0.55,10000,"BadPrincessHud/RangerPortrait",""))
	AddActionNode(ShowMessageAction("A","WHAT!! How they dare!! I thought it was clear enough that I dislike ANY kind of visitors...",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(ShowMessageAction("A","Very well, they are going to regret having took a single step in MY domains!",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(ShowMessageAction("A","Quick, call the warriors!! We will teach those intruders a lesson...",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))	
end

function summonWarriors()	
	
	AddActionNode(ShowMessageAction("A","Warriors summoned: 0 / 4",0.8,0.2,0,"","objective"))
	local a1 = EntityAction("WarriorGoblin",1)
	a1:AddEndActionNode(RemoveMessageAction("A"))	
	AddActionNode(a1)
	
	AddActionNode(ShowMessageAction("A","Warriors summoned: 1 / 4",0.8,0.2,0,"","objective"))
	a1 = EntityAction("WarriorGoblin",1)
	a1:AddEndActionNode(RemoveMessageAction("A"))	
	AddActionNode(a1)
	
	AddActionNode(ShowMessageAction("A","Warriors summoned: 2 / 4",0.8,0.2,0,"","objective"))
	a1 = EntityAction("WarriorGoblin",1)
	a1:AddEndActionNode(RemoveMessageAction("A"))	
	AddActionNode(a1)
	
	AddActionNode(ShowMessageAction("A","Warriors summoned: 3 / 4",0.8,0.2,0,"","objective"))
	a1 = EntityAction("WarriorGoblin",1)
	a1:AddEndActionNode(RemoveMessageAction("A"))	
	AddActionNode(a1)
	
end

function attackCamp()

	local mess = ActivateCamp(1,300000)
	local a = SendMessageAction("Camp",mess)
	AddActionNode(a)
	
	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))	
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(ShowMessageAction("A","Here they are... Now! Let's purge that camp!",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))	
	
	AddActionNode(ShowMessageAction("A","Kill the intruders!",0.8,0.2,0,"","objective"))

	local a1 = CheckWaveEndedAction(1)
	a1:AddEndActionNode(RemoveMessageAction("A"))	
	AddActionNode(a1)

end
	
function defendPrincess()
	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))	
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(ShowMessageAction("A","Ahhhh, I could do this the whole day...",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(ShowMessageAction("A","Ok, let's go back to the castle.",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))		
	
	AddActionNode(WaitTimeAction(10000))
	
	AddActionNode(ShowMessageAction("A","Your highness!! It seems that one of the intruders wasn't present during our raid to the camp.",0.55,0.55,10000,"BadPrincessHud/RangerPortrait",""))
	AddActionNode(ShowMessageAction("A","He escaped and has called for reinforcements, they will be here soon..",0.55,0.55,10000,"BadPrincessHud/RangerPortrait",""))
	AddActionNode(ShowMessageAction("A","Great, it's almost dinner time...",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
		
	AddActionNode(TimerAction("FFFAFA00",120000))
	
	AddActionNode(DefendToWaveAction(2,60000))
	AddActionNode(DefendToWaveAction(3,-1))	
	
	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))	
	AddActionNode(MoveCameraAction("Princess"))	
	AddActionNode(ShowMessageAction("A","That was too easy... even coming from them... I wonder if they are planning something else...",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))		

end

function defendForCertainTime()
--los parametro son en orden:
-- tiempo para cada unidad por calcular el tiempo maximo de la oleada
-- tiempo de spawn entre cada enemigo
-- espacio inicial de la mocilla
-- incremento de la mocilla por cada oleada
-- tiempo maximo de duracion de l'action o sea el timpo que tiene que aguantar
 
	local b = TimedSurvivorAction(5000,1000,5,2,45000)
	b:AddEndActionNode(KillAllEnemiesAction())
	AddActionNode(b)

end 