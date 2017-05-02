function loadGameAction()	

	introduction()
	startBattle()
end

function init()
	disableAllClicks()
	disableAllAbilities()
	AddActionNode(MoveCameraAction("Princess"))
	AddActionNode(BlockCameraAction(true))
end

function disableAllClicks()
	AddActionNode(SetMouseIsLockedAction(true))
	
	AddActionNode(SetClickIsEnabledAction(1,"Ground",false))
	AddActionNode(SetClickIsEnabledAction(1,"Princess",false))
	AddActionNode(SetClickIsEnabledAction(1,"Well",false))
	AddActionNode(SetClickIsEnabledAction(1,"WorkmanGoblin",false))
	
	AddActionNode(SetClickIsEnabledAction(2,"Ground",false))
	AddActionNode(SetClickIsEnabledAction(2,"Tree",false))
	AddActionNode(SetClickIsEnabledAction(2,"Rock",false))
	AddActionNode(SetClickIsEnabledAction(2,"Iron",false))
end

function disableAllAbilities()

	AddActionNode(SetAbilityIsEnabledAction("WorkmanGoblin","Q",false))
	AddActionNode(SetAbilityIsEnabledAction("WorkmanGoblin","W",false))
	AddActionNode(SetAbilityIsEnabledAction("WorkmanGoblin","E",false))
	
	AddActionNode(SetAbilityIsEnabledAction("Well","Q",false))
	AddActionNode(SetAbilityIsEnabledAction("Well","W",false))
	AddActionNode(SetAbilityIsEnabledAction("Well","E",false))
	
	AddActionNode(SetAbilityIsEnabledAction("Princess","Q",false))
	AddActionNode(SetAbilityIsEnabledAction("Princess","W",false))
	AddActionNode(SetAbilityIsEnabledAction("Princess","E",false))
	AddActionNode(SetAbilityIsEnabledAction("Princess","R",false))
	
	AddActionNode(SetAbilityIsEnabledAction("RangerGoblin","Q",false))
	AddActionNode(SetAbilityIsEnabledAction("RangerGoblin","W",false))
	AddActionNode(SetAbilityIsEnabledAction("RangerGoblin","E",false))
	
end
function enableAllClicks()
	AddActionNode(SetMouseIsLockedAction(false))
	
	AddActionNode(SetClickIsEnabledAction(1,"Ground",true))
	AddActionNode(SetClickIsEnabledAction(1,"Princess",true))
	AddActionNode(SetClickIsEnabledAction(1,"Well",true))
	AddActionNode(SetClickIsEnabledAction(1,"WorkmanGoblin",true))
	
	AddActionNode(SetClickIsEnabledAction(2,"Ground",true))
	AddActionNode(SetClickIsEnabledAction(2,"Tree",true))
	AddActionNode(SetClickIsEnabledAction(2,"Rock",true))
	AddActionNode(SetClickIsEnabledAction(2,"Iron",true))
end

function enableAllAbilities()

	AddActionNode(SetAbilityIsEnabledAction("WorkmanGoblin","Q",true))
	AddActionNode(SetAbilityIsEnabledAction("WorkmanGoblin","W",true))
	AddActionNode(SetAbilityIsEnabledAction("WorkmanGoblin","E",true))
	
	AddActionNode(SetAbilityIsEnabledAction("Well","Q",true))
	AddActionNode(SetAbilityIsEnabledAction("Well","W",true))
	AddActionNode(SetAbilityIsEnabledAction("Well","E",true))
	
	AddActionNode(SetAbilityIsEnabledAction("Princess","Q",true))
	AddActionNode(SetAbilityIsEnabledAction("Princess","W",true))
	AddActionNode(SetAbilityIsEnabledAction("Princess","E",true))
	AddActionNode(SetAbilityIsEnabledAction("Princess","R",true))
	
	AddActionNode(SetAbilityIsEnabledAction("RangerGoblin","Q",true))
	AddActionNode(SetAbilityIsEnabledAction("RangerGoblin","W",true))
	AddActionNode(SetAbilityIsEnabledAction("RangerGoblin","E",true))
	
end

function introduction()

	AddActionNode(ShowMessageAction("A","Welcome to the Bad Princess Survival mode (press space to skip...)",0.55,0.55,20000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(ShowMessageAction("A","Do you already know how to play Bad Princess? If you don't, I strongly recommend you to play the tutorial first.",0.55,0.55,20000,"BadPrincessHud/LumberjackPortrait",""))
	
	AddActionNode(ShowMessageAction("A","In this game mode, you have to use all your skill to protect the Princess.",0.55,0.55,20000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(ShowMessageAction("A","There aren't any restriction, all is valid.",0.55,0.55,20000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(ShowMessageAction("A","The enemies are ruthless and will arrive over and over again until they succeed in kidnapping the Princess.",0.55,0.55,20000,"BadPrincessHud/LumberjackPortrait",""))
	
	AddActionNode(ShowMessageAction("A","How long would you be able to push back the enemy?",0.55,0.55,20000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(ShowMessageAction("A","Here there is your score and now..",0.3,0.1,20000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(ShowMessageAction("A","Show us your courage!!",0.55,0.55,20000,"BadPrincessHud/LumberjackPortrait",""))
	
		
end

function startBattle()

	AddActionNode(TimerAction("FFFAFA00",60000))
	AddActionNode(SurvivorAction(15000,3000,5,30000))
end