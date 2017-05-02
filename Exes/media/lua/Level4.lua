function loadGameAction()

	introduction()
	summonRangers()
	summonWarriors()
	headingToTheCave()
	backToTheCastle()
	repelAssault()
		
	local m = GameOver(true)
	local b = SendMessageAction("GameManager",m)
	AddActionNode(b)
	
end

function introduction()

	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))
	
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(ShowMessageAction("A","Any news about some stupid platoon of knights trying to shorten their pathetic lives? (Push SPACE to continue...)",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(ShowMessageAction("A","No, your grace. No news so far.",0.55,0.55,10000,"BadPrincessHud/RangerPortrait",""))
	AddActionNode(ShowMessageAction("A","Then we have free pass in our quest for the moment. Let's see which is our next destination.",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(ShowMessageAction("A","Mmmm... the Dark Cave... I went there once with my father when I was a child, but he told me not to ever go there alone, for it is said that is a cursed place...",0.55,0.55,20000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(ShowMessageAction("A","Well! It happens that I'm not a child anymore, and I'm not afraid of cursed places... I actually enjoy them!",0.55,0.55,20000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(ShowMessageAction("A","We better get ready for the travel, and then... to the Bat-Cave!!... I mean... To the Dark Cave!!",0.55,0.55,20000,"BadPrincessHud2/PrincessPortrait",""))
	
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))	
	
end


function summonRangers()	
	
	AddActionNode(ShowMessageAction("A","Rangers summoned: 0 / 3",0.8,0.2,0,"","objective"))
	local a1 = EntityAction("RangerGoblin",1)
	a1:AddEndActionNode(RemoveMessageAction("A"))	
	AddActionNode(a1)
	
	AddActionNode(ShowMessageAction("A","Rangers summoned: 1 / 3",0.8,0.2,0,"","objective"))
	a1 = EntityAction("RangerGoblin",1)
	a1:AddEndActionNode(RemoveMessageAction("A"))	
	AddActionNode(a1)
	
	AddActionNode(ShowMessageAction("A","Rangers summoned: 2 / 3",0.8,0.2,0,"","objective"))
	a1 = EntityAction("RangerGoblin",1)
	a1:AddEndActionNode(RemoveMessageAction("A"))	
	AddActionNode(a1)
	
end

function summonWarriors()	
	
	AddActionNode(ShowMessageAction("A","Warriors summoned: 0 / 3",0.8,0.2,0,"","objective"))
	local a1 = EntityAction("WarriorGoblin",1)
	a1:AddEndActionNode(RemoveMessageAction("A"))	
	AddActionNode(a1)
	
	AddActionNode(ShowMessageAction("A","Warriors summoned: 1 / 3",0.8,0.2,0,"","objective"))
	a1 = EntityAction("WarriorGoblin",1)
	a1:AddEndActionNode(RemoveMessageAction("A"))	
	AddActionNode(a1)
	
	AddActionNode(ShowMessageAction("A","Warriors summoned: 2 / 3",0.8,0.2,0,"","objective"))
	a1 = EntityAction("WarriorGoblin",1)
	a1:AddEndActionNode(RemoveMessageAction("A"))	
	AddActionNode(a1)
	

end

function headingToTheCave()

	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))
	
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(ShowMessageAction("A","We are ready. The cave is in the south-west of the castle, passing the river. (Push SPACE to continue...)",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))	
	
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))	
	
	AddActionNode(ShowMessageAction("A","Go to the Dark Cave.",0.8,0.2,0,"","objective"))	
	
	local mess = ActivateCamp(4,300000)
	local a = SendMessageAction("Camp",mess)
	AddActionNode(a)
	
	mess = ActivateCamp(5,300000)
	a = SendMessageAction("Camp2",mess)
	AddActionNode(a)
	

	
	mess = ActivateCamp(7,300000)
	a = SendMessageAction("Camp4",mess)
	AddActionNode(a)	
		
	mess = ActivateCamp(8,300000)
	a = SendMessageAction("Camp5",mess)
	AddActionNode(a)
	
	a1 = CheckDistanceBetweenEntitiesAction("Princess","Camp3",31)
	a1:AddEndActionNode(RemoveMessageAction("A"))	
	AddActionNode(a1)
			
	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))
	
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(ShowMessageAction("A","Well well.. it seems that I am not alone after all... (Push SPACE to continue...)",0.55,0.55,10000,"BadPrincessHud/RangerPortrait",""))	
	AddActionNode(ShowMessageAction("A","For the princess!!",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))	
	
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))	

	mess = ActivateCamp(6,300000)
	a = SendMessageAction("Camp3",mess)
	AddActionNode(a)
	
	a1 = CheckWaveEndedAction(6)
	a1:AddStartAsyncActionNode(ShowMessageAction("A","Kill the attackers!",0.8,0.2,0,"","objective"))
	a1:AddEndActionNode(RemoveMessageAction("A"))	
	AddActionNode(a1)
--[[
	a1 = EntityAction("Assassin",0)
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(EntityAction("Knight",0))
	AddActionNode(a1)
]]--	
	
	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))
	
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(ShowMessageAction("A","The other piece of the sceptre must be somewhere in the cave... (Push SPACE to continue...)",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))		
	
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))	
	
	AddActionNode(ShowMessageAction("A","Look for the piece of the sceptre.",0.8,0.2,0,"","objective"))	
	
	AddActionNode(CreateEntityAction("RodHead","Camp3"))
	
	a1 = CheckDistanceBetweenEntitiesAction("Princess","RodHead0",10)
	a1:AddEndActionNode(RemoveMessageAction("A"))	
	AddActionNode(a1)
	
	AddActionNode(DeleteEntityAction("RodHead0"))	
	
	
	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))
	
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(ShowMessageAction("A","Hahahaha!!! (Push SPACE to continue...)",0.55,0.55,10000,"",""))	
	AddActionNode(ShowMessageAction("A","????",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))	
	AddActionNode(ShowMessageAction("A","I knew that you would lead me to the hidden treasure!",0.55,0.55,10000,"",""))	
	AddActionNode(ShowMessageAction("A","Who are you!!",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))	
	AddActionNode(ShowMessageAction("A","My name is Roger Roberts, and the king of Cydonia has paid me a good amount of money to get that magical Sceptre.",0.55,0.55,20000,"BadPrincessHud/AssassinCounter",""))
	AddActionNode(ShowMessageAction("A","Now, be a good girl and give me that piece of the sceptre.",0.55,0.55,10000,"BadPrincessHud/AssassinCounter",""))	
	AddActionNode(ShowMessageAction("A","Come and take it by yourself...",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))	
	
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))	
	
	AddActionNode(CreateEntityAction("AssassinBoss","Camp10"))
	
	AddActionNode(ShowMessageAction("A","Kill the master assassin!",0.8,0.2,0,"","objective"))
	
	a1 = KillAllEnemiesAction()
	a1:AddEndActionNode(RemoveMessageAction("A"))	
	AddActionNode(a1)
	
end

function backToTheCastle()

	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))
	
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(ShowMessageAction("A","Phew... they are starting to take that marriage matter very seriously... we must hurry in completing the Sceptre.",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))	
	AddActionNode(ShowMessageAction("A","Let's back to the castle, the sooner I get the sceptre, the sooner this will end.",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))	
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))	
	
	AddActionNode(ShowMessageAction("A","Go back to the Well Room in the Castle.",0.8,0.2,0,"","objective"))
	
	local mess = ActivateCamp(9,300000)
	local a = SendMessageAction("Camp6",mess)
	AddActionNode(a)
	
	mess = ActivateCamp(10,300000)
	a = SendMessageAction("Camp7",mess)
	AddActionNode(a)
	
	mess = ActivateCamp(11,300000)
	a = SendMessageAction("Camp8",mess)
	AddActionNode(a)
	
	mess = ActivateCamp(12,300000)
	a = SendMessageAction("Camp9",mess)
	AddActionNode(a)		
	
	local a1 = CheckDistanceBetweenEntitiesAction("Princess","WellBase",20)
	a1:AddEndActionNode(RemoveMessageAction("A"))	
	AddActionNode(a1)

end
	

function repelAssault()	

	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))
	
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(ShowMessageAction("A","Your grace!!! A huge army is coming!! I can see different banners!!",0.55,0.55,10000,"BadPrincessHud/RangerPortrait",""))	
	AddActionNode(ShowMessageAction("A","It looks like all the neighbour kingdoms have allied in order to get that sceptre... and me, after that.",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))	
	AddActionNode(ShowMessageAction("A","Prepare the defenses!! Night is about to start, if we push them back long enough, they will stop attacking until the morning...",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))	
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))	
	
	AddActionNode(TimerAction("FFFAFA00",180000))
	
	defendForCertainTime()
	
end

function defendForCertainTime()
--los parametro son en orden:
-- tiempo para cada unidad por calcular el tiempo maximo de la oleada
-- tiempo de spawn entre cada enemigo
-- espacio inicial de la mocilla
-- incremento de la mocilla por cada oleada
-- tiempo maximo de duracion de l'action o sea el timpo que tiene que aguantar
 
	local b = TimedSurvivorAction(10000,1000,5,1,240000)
	b:AddEndActionNode(KillAllEnemiesAction())
	AddActionNode(b)
	
	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))
	
	AddActionNode(MoveCameraAction("Princess"))	
	AddActionNode(ShowMessageAction("A","*pant* *pant* It seems over for now... I will start the spell to forge the Sceptre with its pieces.",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))	
	AddActionNode(ShowMessageAction("A","You, little demons, better rest tonight... tomorrow is going to be a looong day...",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))	
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))	

end 
 