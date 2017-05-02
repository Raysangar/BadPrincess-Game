function loadGameAction()

	introduction()
	repelAssault()
	readingTheMap()
	headingToTheForest()
	backToTheCastle()
		
	local m = GameOver(true)
	local b = SendMessageAction("GameManager",m)
	AddActionNode(b)
	
end

function introduction()

	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))
	
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(ShowMessageAction("A","Your grace! The enemy must have receive news about your plans and wants to impede it, for they are in the way for an early assault today! (Push SPACE to continue...)",0.55,0.55,10000,"BadPrincessHud/RangerPortrait",""))
	AddActionNode(ShowMessageAction("A","Perfect! Just in time for breakfast!",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(ShowMessageAction("A","They will be here in four minutes!",0.55,0.55,10000,"BadPrincessHud/RangerPortrait",""))
	
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))	
	
end

function repelAssault()	
	
	AddActionNode(TimerAction("FFFAFA00",240000))
	
	AddActionNode(DefendToWaveAction(1,60000))
	AddActionNode(DefendToWaveAction(2,60000))
	AddActionNode(DefendToWaveAction(3,-1))
	
end

function readingTheMap()		
	
	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))
	
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(ShowMessageAction("A","Well... according to the map, the sceptre was divided into two parts. One of them should be in the south-eastern forest. (Push SPACE to continue...)",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))	
	
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

function headingToTheForest()

	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))
	
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(ShowMessageAction("A","Quickly, let's heading to the forest! (Push SPACE to continue...)",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))	
	
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))	
	
	AddActionNode(ShowMessageAction("A","Go deep into the south-eastern forest.",0.7,0.2,0,"","objective"))	
	
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
	AddActionNode(ShowMessageAction("A","Your grace! As you said, the forest is not uninhabited! (Push SPACE to continue...)",0.55,0.55,10000,"BadPrincessHud/RangerPortrait",""))	
	AddActionNode(ShowMessageAction("A","We are under attack!",0.55,0.55,10000,"BadPrincessHud/RangerPortrait",""))	
	
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))	

		mess = ActivateCamp(6,300000)
	a = SendMessageAction("Camp3",mess)
	AddActionNode(a)
	
	local a1 = CheckWaveEndedAction(6)
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
	AddActionNode(ShowMessageAction("A","Ok, there must been some part of the sceptre over here... (Push SPACE to continue...)",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))		
	
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))	
	
	AddActionNode(ShowMessageAction("A","Look for the piece of the sceptre.",0.8,0.2,0,"","objective"))	
	
	AddActionNode(CreateEntityAction("RodHandler","Camp3"))
	
	a1 = CheckDistanceBetweenEntitiesAction("Princess","RodHandler0",10)
	a1:AddEndActionNode(RemoveMessageAction("A"))	
	AddActionNode(a1)
	
	AddActionNode(DeleteEntityAction("RodHandler"))
		
	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))
	
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(ShowMessageAction("A","Here it is! The handle of the Sceptre... What a beautiful piece of art... (Push SPACE to continue...)",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))		
	AddActionNode(ShowMessageAction("A","Quickly!! Let's go back to the castle! The map will point us our next destination!",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))	
	
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))	
	

	
end

function backToTheCastle()

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
	


 