function loadGameAction()

	introduction()
	summonRangers()
	summonWarriors()
	checkCamp()
	defendPrincess()
	findMap()

		
	local m = GameOver(true)
	local b = SendMessageAction("GameManager",m)
	AddActionNode(b)
end

function introduction()

	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))
	
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(ShowMessageAction("A","Your grace! One of the knights in the last assault has dropped this letter. It seems important... (Push SPACE to continue...)",0.55,0.55,10000,"BadPrincessHud/RangerPortrait",""))
	AddActionNode(ShowMessageAction("A","And what are you waiting for? Read it to me!!",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(ShowMessageAction("A","But... your grace, we goblins do not know how to read...",0.55,0.55,10000,"BadPrincessHud/RangerPortrait",""))
	AddActionNode(ShowMessageAction("A","Huh, you useless minion... Give it to me!!",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(ShowMessageAction("A","Hmmmm, interesting... According this, they are looking for some kind of weapon empowered with dark magic... but the rest of the letter is missing...",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(ShowMessageAction("A","Your grace, maybe there is more information in the eastern camp...",0.55,0.55,10000,"BadPrincessHud/RangerPortrait",""))
	AddActionNode(ShowMessageAction("A","I know!!! Let's take a look in the eastern camp, maybe there is more information about this weapon.",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(ShowMessageAction("A","...",0.55,0.55,10000,"BadPrincessHud/RangerPortrait",""))
	
	AddActionNode(ShowMessageAction("A","Well, we will need some muscle for that. Quickly, summon some warriors and rangers.",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))	
end

function summonRangers()	
	
	AddActionNode(ShowMessageAction("A","Rangers summoned: 0 / 2",0.8,0.2,0,"","objective"))
	local a1 = EntityAction("RangerGoblin",1)
	a1:AddEndActionNode(RemoveMessageAction("A"))	
	AddActionNode(a1)
	
	AddActionNode(ShowMessageAction("A","Rangers summoned: 1 / 2",0.8,0.2,0,"","objective"))
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

function checkCamp()

	local mess = ActivateCamp(1,300000)
	local a = SendMessageAction("Camp",mess)
	AddActionNode(a)
	
	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))	
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(ShowMessageAction("A","Well, let's go! It's almost dawn, and the daily ration of raiders is about to arrive...",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))	
	
	AddActionNode(ShowMessageAction("B","Check the camp.",0.8,0.2,0,"","objective"))
	local a1 = EntityAction("Assassin",1)
	a1:AddEndActionNode(RemoveMessageAction("B"))
	AddActionNode(a1)
	
	AddActionNode(ShowMessageAction("A","Kill the attackers!",0.8,0.2,0,"","objective"))
	local a1 = CheckWaveEndedAction(1)
	a1:AddEndActionNode(RemoveMessageAction("A"))	
	AddActionNode(a1)
--[[
	a1 = EntityAction("Assassin",0)
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(EntityAction("Knight",0))
	AddActionNode(a1)
]]--	
	AddActionNode(CreateEntityAction("Scroll","Camp"))
	AddActionNode(ShowMessageAction("B","Look for information.",0.8,0.2,0,"","objective"))
	a1 = CheckDistanceBetweenEntitiesAction("Princess","Scroll0",10)
	a1:AddEndActionNode(RemoveMessageAction("B"))	
	AddActionNode(a1)
	AddActionNode(DeleteEntityAction("Scroll0"))
	
end
	
function defendPrincess()
	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))	
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(ShowMessageAction("A","Well well... This is definitely very interesting...",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(ShowMessageAction("A","It turns out that there is some kind of magical sceptre hidden in MY lands. This sceptre has the power to break the will of the target.",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(ShowMessageAction("A","Apparently, they wanted to use it against me, to force me to get married with some stupid knight...",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(ShowMessageAction("A","But they are about to get backfired... For I am going to find it before them, and use it to make them leave me alone.",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(ShowMessageAction("A","I remember a old map that my father used to keep quite fervently in the castle.",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(ShowMessageAction("A","Let's go back and see whether it can help me.",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))		
	
	AddActionNode(WaitTimeAction(5000))
	
	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))	
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(ShowMessageAction("A","Your highness!! Our explorers reported that the army of today's attempt are comming!!",0.55,0.55,10000,"BadPrincessHud/RangerPortrait",""))	
	AddActionNode(ShowMessageAction("A","Well, the map will have to wait.",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))	
	
	AddActionNode(TimerAction("FFFAFA00",120000))
	
	AddActionNode(DefendToWaveAction(2,180000))
	AddActionNode(DefendToWaveAction(3,-1))
end	

function findMap()

	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))	
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(ShowMessageAction("A","It's over for today... Quickly, bring me the map!!",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(ShowMessageAction("A","Your highness... there is a problem...",0.55,0.55,10000,"BadPrincessHud2/WarriorPortrait",""))	
	AddActionNode(ShowMessageAction("A","... so?...",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(ShowMessageAction("A","It seems that during the last attack, some thief sneaked in the castle and stole the map...",0.55,0.55,10000,"BadPrincessHud2/WarriorPortrait",""))	
	AddActionNode(ShowMessageAction("A","... ...",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(ShowMessageAction("A","But another camp in the east of here has been spotted!! The thief should probably have gone there!!",0.55,0.55,10000,"BadPrincessHud2/WarriorPortrait",""))	
	AddActionNode(ShowMessageAction("A","... ...",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))	
	
	
	local mess = ActivateCamp(4,600000)
	local a = SendMessageAction("Camp2",mess)
	AddActionNode(a)
	
	AddActionNode(ShowMessageAction("B","Check the eastern camp.",0.8,0.2,0,"","objective"))
	local a1 = EntityAction("Assassin",1)
	a1:AddEndActionNode(RemoveMessageAction("B"))
	AddActionNode(a1)	
	
	AddActionNode(ShowMessageAction("A","Kill the attackers!",0.8,0.2,0,"","objective"))
--[[
	a1 = EntityAction("Assassin",0)
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(EntityAction("Knight",0))
	AddActionNode(a1)
]]--
	a1 = CheckWaveEndedAction(4)
	a1:AddEndActionNode(RemoveMessageAction("A"))	
	AddActionNode(a1)
	
	AddActionNode(CreateEntityAction("Scroll2","Camp2"))
	AddActionNode(ShowMessageAction("B","Find the map.",0.8,0.2,0,"","objective"))
	a1 = CheckDistanceBetweenEntitiesAction("Princess","Scroll20",10)
	a1:AddEndActionNode(RemoveMessageAction("B"))	
	AddActionNode(a1)
	
	AddActionNode(DeleteEntityAction("Scroll20"))
	
	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(BlockCameraAction(true))	
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(ShowMessageAction("A","Tt's no crime to steal from a thief... ",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))
	AddActionNode(ShowMessageAction("A","Let's back to business, this is going to be fun!",0.55,0.55,10000,"BadPrincessHud2/PrincessPortrait",""))		
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(BlockCameraAction(false))	
	
end


 