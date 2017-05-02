function loadGameAction()

	introduction()
	bigBattle()
	takeTheSceptre()
	killTheEnemyKings()
	ending()
			
	local m = GameOver(true)
	local b = SendMessageAction("GameManager",m)
	AddActionNode(b)
	
end

function introduction()

	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))
	
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(ShowMessageAction("A","Well, it took the whole night, but the spell is ready... All it needs now is get charged with the magic of the Dark Well... (Push SPACE to continue...)",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(ShowMessageAction("A","Your grace! The sun is about to rise up, and the enemy is waiting beyond the river for that moment.",0.55,0.55,10000,"BadPrincessHud/RangerPortrait",""))
	AddActionNode(ShowMessageAction("A","Damn them!! We will have to fight and stand long enough for the Sceptre to be ready... ",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(ShowMessageAction("A","CALL TO ARMS!!! If they want war, war will they have...",0.55,0.55,20000,"BadPrincessHud2/PrincessPortrait",""))
	
	
	
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))	
	
end


function bigBattle()	
	
	AddActionNode(ShowMessageAction("A","Prepare for the battle!!",0.8,0.2,0,"","objective"))
	local a1 = TimerAction("FFFAFA00",240000)
	a1:AddEndActionNode(RemoveMessageAction("A"))	
	AddActionNode(a1)
	
	AddActionNode(ShowMessageAction("A","Repel the enemy!",0.8,0.2,0,"","objective"))
	defendForCertainTime()
	AddActionNode(RemoveMessageAction("A"))
	
end


function takeTheSceptre()

	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))
	
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(ShowMessageAction("A","The Sceptre is complete!!(Push SPACE to continue...)",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))	
	
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))	
	
	AddActionNode(ShowMessageAction("A","Take the sceptre.",0.8,0.2,0,"","objective"))	
	
	local a1 = CheckDistanceBetweenEntitiesAction("Princess","RodHead",10)
	a1:AddEndActionNode(RemoveMessageAction("A"))	
	AddActionNode(a1)	
		
	AddActionNode(DeleteEntityAction("RodHead"))
	AddActionNode(DeleteEntityAction("RodHandler"))
	
	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))
	
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(ShowMessageAction("A","I FEEL..... THE POWER.....",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))		
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false)	

	AddActionNode(ChangeAttackAction("Princess",100))	
	
end

function killTheEnemyKings()

	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))
	
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(ShowMessageAction("A","I'LL FACE THOSE TIRESOME KINGS IN PERSON. I WILL TEACH THEM HOW TO TREAT A LADY.... (Push SPACE to continue...)",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))		
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))	
	
	AddActionNode(ShowMessageAction("A","Kill the kings.",0.8,0.2,0,"","objective"))

	AddActionNode(CreateEntityAction("KightBoss","SpawnPoint1"))
	AddActionNode(CreateEntityAction("KightBoss","SpawnPoint2"))
	AddActionNode(CreateEntityAction("KightBoss","SpawnPoint3"))
	
	AddActionNode(EntityAction("Knight",0))	

end
	

function ending()	

	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))
	
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(ShowMessageAction("A","THIS IS NOT OVER... THEY WANTED ME IN THEIR KINGDOMS, AND THEY WILL HAVE ME THERE... (Push SPACE to continue...)",0.55,0.55,10000,"BadPrincessHud/RangerPortrait",""))	
	AddActionNode(ShowMessageAction("A","FOR WITH THE POWER THAT THIS SCEPTRE GRANTS ME, I WILL CONQUER THEIR KINGDOMS... ONE BY ONE..",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))		
	
	AddActionNode(MoveCameraAction("Forest2Tree1"))
	
	AddActionNode(ShowMessageAction("A","Oh oh...",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))
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
 
	local b = TimedSurvivorAction(10000,1000,5,1,300000)
	b:AddEndActionNode(KillAllEnemiesAction())
	AddActionNode(b)


end 
 