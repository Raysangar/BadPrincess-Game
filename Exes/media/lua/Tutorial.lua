function loadGameAction()	


	init()
	introduction()
	unitSelection()
	movingUnits()
	gatherStone()
	summonWorkman()
	storyTime()
	firstBattle()
	carryChestAndGatherIron()
	summonWarriors()
	secondBattle()	
	firstLooter()
	introduceRangers()
	gatherWood()
	summonRangers()	
	createTraps()
	bigWaveWarning()
	bigWave()
	assassinIntroduction()
	lastWave()
	ending()
	
	local m = GameOver(true)
	local b = SendMessageAction("GameManager",m)
	AddActionNode(b)

end

function init()
-- GIGUI
	AddActionNode(IncrementsWavesCounter(0,1))
	disableAllClicks()
	disableAllAbilities()
	AddActionNode(MoveCameraAction("Forest2Tree0"))
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
	AddActionNode(SetClickIsEnabledAction(2,"Well",false))
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

	AddActionNode(ShowMessageAction("A","Welcome to the great kingdom of Galeonia! (Push Space to continue...)",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(ShowMessageAction("A","Our beloved Princess has 'asked' me to lead you through your first steps in the hard commitment that is protecting her. She hasn't obliged me at all, I am here voluntarily... ",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))
	
	AddActionNode(ShowMessageAction("A","My name is Leon, The Professional... lumberjack.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(ShowMessageAction("A","Do not try to look for me, for I am hidden in the woods, but it has nothing to do with those totally peaceful goblins... I swear.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))
	
	local a1 = MoveCameraAction("Princess")
	a1:AddEndActionNode(ShowMessageAction("A","But first goes first, let me introduce you... THE PRINCESS!",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(a1)
	AddActionNode(ShowMessageAction("A","As you can see, she is accompanied by his trustful minion... SERVANT! I mean servant...",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))
	
		
end

function unitSelection()

	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(SetClickIsEnabledAction(1,"Princess",true))
	local entities = StringList()
	entities:push_back("Princess")		
	local a1 = CheckSelectedUnitsAction(entities)
	a1:AddStartActionNode(ShowMessageAction("A","Try to click on the Princess",0.55,0.55,0,"BadPrincessHud/LumberjackPortrait",""))
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a1)
	
	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(ShowMessageAction("A","This is our unbelievably glorious Princess.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(ShowMessageAction("A","She can help you in the battle by using her powers, or using her Aura of Darkness to place new Wells of Darkness over it.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))
	
	
	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(SetClickIsEnabledAction(1,"WorkmanGoblin",true))
	entities = StringList()
	entities:push_back("WorkmanGoblin1")
	a1 = CheckSelectedUnitsAction(entities)
	a1:AddStartActionNode(ShowMessageAction("A","Now, try to click on the Goblin",0.55,0.55,0,"BadPrincessHud/LumberjackPortrait",""))
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a1)
	
	AddActionNode(SetMouseIsLockedAction(true))
	AddActionNode(ShowMessageAction("A","This is one of the Princess' subjects. Specifically, this is a Workman Goblin.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(ShowMessageAction("A","It can gather resources, and create or repair Wells of Darkness, which are used to summon more goblins.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))
	
	AddActionNode(ShowMessageAction("A","This big portrait here down represents the current active unit. This is, between all your selected units, the one which you can use abilities of.",0.25,0.60,10000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(ShowMessageAction("A","You can also see its current Health Points.",0.25,0.60,10000,"BadPrincessHud/LumberjackPortrait",""))

	AddActionNode(SetClickIsEnabledAction(1,"Ground",true))
	AddActionNode(SetMouseIsLockedAction(false))
	
	entities = StringList()
	entities:push_back("WorkmanGoblin1")
	entities:push_back("Princess")
	a1 = CheckSelectedUnitsAction(entities)
	a1:AddStartActionNode(ShowMessageAction("A","Click on the ground and drag the mouse to select more than one unit simultaneously.",0.55,0.55,0,"BadPrincessHud/LumberjackPortrait",""))
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a1)
	
	AddActionNode(SetMouseIsLockedAction(true))
	
	AddActionNode(ShowMessageAction("A","Under  the ability buttons you can see two little portraits. Those are the selected units.",0.25,0.60,10000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(ShowMessageAction("A","You can command them to attack, or move, but you can't use their abilities, except for the framed one, which is the active unit.",0.25,0.60,10000,"BadPrincessHud/LumberjackPortrait",""))

end

function movingUnits()

	AddActionNode(SetMouseIsLockedAction(false))
	AddActionNode(SetClickIsEnabledAction(2,"Ground",true))
	AddActionNode(BlockCameraAction(false))
	AddActionNode(ShowMessageAction("A","To command a unit or a group of units to move, do right click on some position.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	
end

function gatherStone()
	
	AddActionNode(SetMouseIsLockedAction(true))	
	AddActionNode(MoveCameraAction("WorkmanGoblin1"))
	AddActionNode(BlockCameraAction(true))
	AddActionNode(UnselectEntitiesAction())
	AddActionNode(ShowMessageAction("A","Now, as you can guess, there isn't much you can do with just one goblin. The problem is that goblins, even being as kind and selfless as they are, need some resources to be summoned.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(ShowMessageAction("A","You will need a few more workmen to increase your resource gathering effectiveness. And Workmen Goblins need 200 units of Stone to be summoned.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(ShowMessageAction("A","I will show you how to gather.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(SetMouseIsLockedAction(false))	
	
	local entities = StringList()
	entities:push_back("WorkmanGoblin1")
	local a1 = CheckSelectedUnitsAction(entities)
	a1:AddStartActionNode(ShowMessageAction("A","First, do left click on the Workman goblin.",0.55,0.55,0,"BadPrincessHud/LumberjackPortrait",""))
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a1)
	
	AddActionNode(MoveCameraAction("Rocks3Rock0"))
	AddActionNode(SetClickIsEnabledAction(2,"Rock",true))
	
	a1 = CheckDistanceBetweenEntitiesAction("WorkmanGoblin1","Rocks3Rock0",16)
	a1:AddStartActionNode(ShowMessageAction("A","This is a rock. Having the Workman selected, do right click on the rock to mine it",0.55,0.55,0,"BadPrincessHud/LumberjackPortrait",""))
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a1)
	
	AddActionNode(FollowEntityAction("WorkmanGoblin1"))
	AddActionNode(ShowMessageAction("A","Now Mike (let’s call him Mike for now), is going to gather some Stone.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(ShowMessageAction("A","Since goblin tailors didn't master the great art of pocket making, the gathered Stone must be stocked somewhere safe.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(WaitTimeAction(2000))
	AddActionNode(ShowMessageAction("A","Let's wait until having 200 units of Stone",0.55,0.55,0,"BadPrincessHud/LumberjackPortrait",""))
	a1 = CheckResourceUnitsAction(2,200)
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a1)
	AddActionNode(FollowEntityAction(""))
end

function summonWorkman()

	AddActionNode(ShowMessageAction("A","Now, as you can see, the sum of available stone up here has increased. And thanks to Mike, you are now able to summon another Workman goblin.",0.13,0.25,10000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(MoveCameraAction("WellBase"))
	AddActionNode(UnselectEntitiesAction())
	AddActionNode(SetClickIsEnabledAction(1,"Well",true))
	
	local entities = StringList()
	entities:push_back("WellBase")
	local a1 = CheckSelectedUnitsAction(entities)
	a1:AddStartActionNode(ShowMessageAction("A","Let’s do it! Left click on the Well.",0.55,0.55,0,"BadPrincessHud/LumberjackPortrait",""))
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a1)
	
	AddActionNode(SetAbilityIsEnabledAction("Well","Q",true))
	
	AddActionNode(ShowMessageAction("A","If you pay attention to the Ability bar, you can see three different portraits. Each one is related to a different class.",0.25,0.60,10000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(ShowMessageAction("A","For now, do click on the first portrait (or press the Q key of your keyboard)to summon a new Workman Goblin.",0.25,0.60,0,"BadPrincessHud/LumberjackPortrait",""))
	
	a1 = CheckDistanceBetweenEntitiesAction("WorkmanGoblin0","WellBase",16)
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a1)
	
	AddActionNode(ShowMessageAction("A","Great... now there are two of them...",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(ShowMessageAction("A","I MEAN… GREAT! THERE ARE TWO OF THEM! I COULDN'T BE HAPPIER!",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(BlockCameraAction(false))
	AddActionNode(ShowMessageAction("A","Select the two Workman goblins (one by one), and place them close to the Princess",0.55,0.55,0,"BadPrincessHud/LumberjackPortrait",""))	
	
	a1 = CheckDistanceBetweenEntitiesAction("WorkmanGoblin0","Princess",30)
	local a2 = CheckDistanceBetweenEntitiesAction("WorkmanGoblin1","Princess",30)
	a1:AddStartActionNode(a2)
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a1)
	
	AddActionNode(SetMouseIsLockedAction(true))	
	
end

function storyTime()
	
	AddActionNode(ShowMessageAction("A","Story time!",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(ShowMessageAction("A","Since the sudden death of her father, King Galeon, the interest of the neighbour realms for the Princess has increased.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(ShowMessageAction("A","These realms started to send Knights who would try to convince the Princess to marry them.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(ShowMessageAction("A","But our not-violent-at-all Princess is quite stubborn, so they put peaceful intentions apart, and now they are trying to capture her by force.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(ShowMessageAction("A","Since she wants to stay single, you must protect her using these little green creatures.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))
	AddActionNode(ShowMessageAction("A","Ok, focus now. Our explorers have reported that a lonely Knight of the realm of Cydonia is brave (or drunk) enough to try to take our beloved Princess to his realm.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))
	
end

function firstBattle()

	AddActionNode(BlockCameraAction(false))
	AddActionNode(SetMouseIsLockedAction(false))
AddActionNode(ShowMessageAction("A","Pro-Tip: ",0.25,0.60,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","Any unit has two stand modes: By default, every Goblin is in Passive mode, represented by a Shield in the button. ",0.25,0.60,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","In this mode, your units will only attack if you command it so, or if they are attacked by an enemy. ",0.25,0.60,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","If you click on the button with the swords in it, the Goblin will change to Aggressive mode, where the unit will attack any close enemy automatically. ",0.25,0.60,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","Aggressive mode is good to watch over areas.",0.25,0.60,10000,"BadPrincessHud/LumberjackPortrait",""))	
	
	local a1 = CreateEntityAction("Knight","SpawnPoint1")
	AddActionNode(a1)
	AddActionNode(ImmortalUnitAction("Knight0",true))
	AddActionNode(ChangeAttackAction("Knight0",0))
	
	
	AddActionNode(ShowMessageAction("A","He will be here soon...",0.55,0.55,0,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(CheckDistanceBetweenEntitiesAction("Knight0","Princess",60))
	AddActionNode(RemoveMessageAction("A"))
	AddActionNode(BlockCameraAction(false))	
	AddActionNode(ShowMessageAction("A","There he is! Quickly, select any unit and do right click on the Knight to attack him!",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	
	AddActionNode(ShowMessageAction("A","Knights are quite tough, so your better chance is to attack him with all your units.",0.55,0.55,0,"BadPrincessHud/LumberjackPortrait",""))	
	a1 = CheckDistanceBetweenEntitiesAction("WorkmanGoblin0","Knight0",18)
	local a2 = CheckDistanceBetweenEntitiesAction("WorkmanGoblin1","Knight0",18)
	local a3 = CheckDistanceBetweenEntitiesAction("Princess","Knight0",18)
	a2:AddStartActionNode(a3)
	a1:AddStartActionNode(a2)
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a1)
	
	AddActionNode(ShowMessageAction("A","I think it's a good moment to talk about the little anger disorder our Princess has. Click on her.",0.55,0.55,0,"BadPrincessHud/LumberjackPortrait",""))	
	local entities = StringList()
	entities:push_back("Princess")		
	a1 = CheckSelectedUnitsAction(entities)
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a1)
	
	AddActionNode(ShowMessageAction("A","This bar below shows the amount of accumulated Wrath that our Princess has at the moment.",0.12,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","The Wrath will grow when the Princess is attacking, receiving damage, or when any Goblin dies within her Aura of Darkness.",0.12,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","As you can see, the Aura of Darkness under the princess also grows with the Wrath.",0.12,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","Wrath can be spent to use Princess's abilities. Each one has its own cost.",0.12,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	
	AddActionNode(ChangeAttackAction("Knight0",15))
	
	AddActionNode(ShowMessageAction("A","Having the Princess selected, push the first ability button or press Q push away the enemy and stun him for a short period of time.",0.25,0.60,0,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(SetAbilityIsEnabledAction("Princess","Q",true))
	AddActionNode(CheckAbilityUsedAction("Princess","Q"))
	AddActionNode(RemoveMessageAction("A"))
	
	AddActionNode(ImmortalUnitAction("Knight0",false))
	
	AddActionNode(ShowMessageAction("A","Now, finish him!",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	
	AddActionNode(EntityAction("Knight",0))	
	
	AddActionNode(ShowMessageAction("A","Well, that one wasn't a tough one. Maybe he was a new recruit.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
end

function carryChestAndGatherIron()
	AddActionNode(ShowMessageAction("A","Anyway, the other Knights won't be as easily defeatable as this one. And you can't defend the Princess only with Workmen.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","So, you should summon Warrior Goblins for that.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	
	AddActionNode(BlockCameraAction(true))
	AddActionNode(MoveCameraAction("IronOresIron0"))
	
	AddActionNode(ShowMessageAction("A","Warrior Goblins require 200 of Iron to be summoned. And Iron can be obtained from ores like this one.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(SetClickIsEnabledAction(2,"Iron",true))
	
	AddActionNode(ShowMessageAction("A","Let's mine some Iron!",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	
	AddActionNode(UnselectEntitiesAction())
	AddActionNode(MoveCameraAction("WorkmanGoblin1"))
	AddActionNode(ShowMessageAction("A","Select the Workman in the center of the screen.",0.55,0.55,0,"BadPrincessHud/LumberjackPortrait",""))	
	
	local entities = StringList()
	entities:push_back("WorkmanGoblin1")
	local a1 = CheckSelectedUnitsAction(entities)
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a1)
	AddActionNode(BlockCameraAction(false))
	
	AddActionNode(ShowMessageAction("A","Now command him to gather from this iron ore.",0.55,0.55,0,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(MoveCameraAction("IronOresIron0"))
	a1 = CheckDistanceBetweenEntitiesAction("WorkmanGoblin1","IronOresIron0",16)
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a1)
	
	
	
	AddActionNode(ShowMessageAction("A","You can make it faster by moving one of the chests to some position closer to the resource you are gathering at the moment.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	
	AddActionNode(SetAbilityIsEnabledAction("WorkmanGoblin","W",true))
	AddActionNode(BlockCameraAction(true))
	AddActionNode(FollowEntityAction("WorkmanGoblin0"))
	

	AddActionNode(ShowMessageAction("A","Select the other workman.",0.25,0.6,0,"BadPrincessHud/LumberjackPortrait",""))	
		
	local entities = StringList()
	entities:push_back("WorkmanGoblin0")
	local a1 = CheckSelectedUnitsAction(entities)
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a1)
	AddActionNode(BlockCameraAction(false))
	
	AddActionNode(ShowMessageAction("A","And now push the second button, the one with the chest icon, or press W.",0.25,0.6,0,"BadPrincessHud/LumberjackPortrait",""))	
	
	a1 = CheckAbilityUsedAction("WorkmanGoblin0","W")
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a1)
	AddActionNode(FollowEntityAction(""))	
	AddActionNode(BlockCameraAction(false))
	
	AddActionNode(ShowMessageAction("A","Now, do right click over the chest.",0.55,0.55,0,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(MoveCameraAction("Chest"))
	a1 = CheckDistanceBetweenEntitiesAction("WorkmanGoblin0","Chest",16)	
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a1)
	
	AddActionNode(ShowMessageAction("A","The Workman can carry the chest to any place at your choice, but, as you can see, it slows him down.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	
	AddActionNode(ShowMessageAction("A","Carry the chest to somewhere close to the mined Iron ore.",0.55,0.55,0,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(MoveCameraAction("IronOresIron0"))
	a1 = CheckDistanceBetweenEntitiesAction("WorkmanGoblin0","IronOresIron0",60)		
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a1)
	
	AddActionNode(ShowMessageAction("A","Now push again over the second button, or press W.",0.25,0.6,0,"BadPrincessHud/LumberjackPortrait",""))		
	local a2 = CheckAbilityUsedAction("WorkmanGoblin0","W")	
	a2:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a2)
	
	AddActionNode(ShowMessageAction("A","Click again over the Iron ore to resume mining.",0.55,0.55,0,"BadPrincessHud/LumberjackPortrait",""))	
	a1 = CheckDistanceBetweenEntitiesAction("WorkmanGoblin1","IronOresIron0",16)		
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a1)
	
	AddActionNode(WaitTimeAction(3000))
end

function summonWarriors()

	AddActionNode(ShowMessageAction("A","You now have enough Iron to summon a Warrior, so let's do it.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	
	AddActionNode(MoveCameraAction("WellBase"))
	AddActionNode(SetAbilityIsEnabledAction("Well","W",true))
	
	AddActionNode(ShowMessageAction("A","Select the well and push in the Warrior Portrait (the second one).",0.25,0.6,0,"BadPrincessHud/LumberjackPortrait",""))	
	local a1 = CheckDistanceBetweenEntitiesAction("WarriorGoblin0","WellBase",16)
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a1)
	
	AddActionNode(ShowMessageAction("A","Warrior Goblins are the bulk of the Princess's army, so you will need more of them.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","Let's summon 4 more.",0.55,0.55,0,"BadPrincessHud/LumberjackPortrait",""))	
	
	a1 = EntityAction("WarriorGoblin",4)
	a1:AddEndActionNode(RemoveMessageAction("A"))	
	AddActionNode(a1)	
	
end

function secondBattle()

	AddActionNode(ShowMessageAction("A","News from our explorers. This time, a little group of Knights is coming.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	

	AddActionNode(IncrementsWavesCounter(1,3))	

	AddActionNode(ShowMessageAction("A","Place our Warriors close to the Princess and prepare for battle!",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
 
	AddActionNode(DefendToWaveAction(1,-1))
	
	AddActionNode(ShowMessageAction("A","Congratulations! Other ones bite the dust!",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
end

function buildWell()

	AddActionNode(ShowMessageAction("A","Eventually, our Princess will need to go to certain places in the reign, some of them far away from the castle.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","In those cases, you will need to create new Wells of Darkness in order not to having your goblins travelling long distances for protecting her.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	
	AddActionNode(MoveCameraAction("WorkmanGoblin1"))
	
	AddActionNode(ShowMessageAction("A","To create a Well, select the Workman Goblin.",0.55,0.55,0,"BadPrincessHud/LumberjackPortrait",""))	
	local entities = StringList()
	entities:push_back("WorkmanGoblin1")
	local a1 = CheckSelectedUnitsAction(entities)
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a1)
		
	AddActionNode(ShowMessageAction("A","A Well of Darkness, as its name indicates, has its root in the very darkness. So, they can only be created under the Aura of Darkness around our Princess.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","Now, push on the first button (or press Q) and place the Well anywhere within our Princess' Aura of Darkness.",0.25,0.60,0,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(SetAbilityIsEnabledAction("WorkmanGoblin","Q",true))
	a1 = CheckDistanceBetweenEntitiesAction("Well0","Princess",16)
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a1)
	
	AddActionNode(ShowMessageAction("A","After a while, a new Well will surge.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","The only Well made of pure darkness is the one inside the castle. That one won't be able to be destroyed by the enemies.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","However, any other Well built by your Workmen are not so pure, so they can be damaged.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","In that case, use the Repair button of your Workmen (the third one) to... well, repair it, obviously.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(SetAbilityIsEnabledAction("WorkmanGoblin","E",true))
	
end

function firstLooter()
	
	AddActionNode(CreateEntityAction("ThiefEnemy","SpawnPoint3"))
	
	local a1 = CheckDistanceBetweenEntitiesAction("ThiefEnemy0","Chest",60)
	AddActionNode(a1)
	
	AddActionNode(BlockCameraAction(true))
	AddActionNode(FollowEntityAction("ThiefEnemy0"))

	AddActionNode(ShowMessageAction("A","WHAT?? What is this?? Why haven't I be reported??",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","Maybe they are too sneaky...",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
		
	AddActionNode(BlockCameraAction(false))
	AddActionNode(FollowEntityAction(""))
	
	AddActionNode(ShowMessageAction("A","Those are Looters. They are not interested in the Princess, nor in marriage. They just look for something to loot.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","As you can see they run directly to their closest chest, and once they've got enough, they run away fast as hell.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","There is nothing you can do to stop them at the moment, they are way too fast for your Warriors...",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","But the next time, we will be prepared...",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
end

function introduceRangers()

	AddActionNode(MoveCameraAction("Princess"))
	
	AddActionNode(ShowMessageAction("A","There a third class of Goblins that will help us to take down those sneaky thieves.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","They are the Rangers, who, armed with bow and arrows and different traps, can help in the battle from the distance",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","Rangers are also our faster unit",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","This time, a Ranger Goblin need 200 Wood to be summoned",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	
end

function gatherWood()

	AddActionNode(SetClickIsEnabledAction(2,"Tree",true))
	AddActionNode(ShowMessageAction("A","Select any Workman Goblin.",0.55,0.55,0,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(MoveCameraAction("WorkmanGoblin1"))
	
	local entities = StringList()
	entities:push_back("WorkmanGoblin")
	local a1 = CheckSelectedTypesAction(entities)
	a1:AddEndActionNode(MoveCameraAction("Forest3Tree0"))
	AddActionNode(a1)
	AddActionNode(RemoveMessageAction("A"))
	
	AddActionNode(ShowMessageAction("A","And now command him to gather Wood from a tree, by doing right click on it.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	

	
	AddActionNode(ShowMessageAction("A","We will create 3 Rangers so, gather 600 units of wood.",0.55,0.55,0,"BadPrincessHud/LumberjackPortrait",""))	
	a1 = CheckResourceUnitsAction(0,600)
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a1)	
	
end

function summonRangers()

	AddActionNode(ShowMessageAction("A","Because there is never too many Goblins... Let's summon a few more!",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(SetAbilityIsEnabledAction("Well","E",true))
	AddActionNode(ShowMessageAction("A","Select any Well and push the button with the portrait of a Goblin Ranger in it. Do it until you have 3 new Rangers",0.55,0.55,0,"BadPrincessHud/LumberjackPortrait",""))	

	local a1 = EntityAction("RangerGoblin",3)
	a1:AddEndActionNode(RemoveMessageAction("A"))	
	AddActionNode(a1)	
	
	AddActionNode(ShowMessageAction("A","Now, let me show you how useful is this unit.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	
end

function createTraps()

	AddActionNode(ShowMessageAction("A","Select one of them.",0.55,0.55,0,"BadPrincessHud/LumberjackPortrait",""))	
	local entities = StringList()
	entities:push_back("RangerGoblin")
	local a1 = CheckSelectedTypesAction(entities)
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a1)
	
	AddActionNode(ShowMessageAction("A","As you can see in the ability bar, there are different kind of traps, each one with its own cost in resources.",0.25,0.65,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","For now, let's focus in the first one, the Explosive Trap.",0.25,0.65,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","This trap will explode as soon as any enemy step over it, dealing area damage.",0.25,0.65,10000,"BadPrincessHud/LumberjackPortrait",""))	
	
	AddActionNode(SetAbilityIsEnabledAction("RangerGoblin","Q",true))
	
	AddActionNode(ShowMessageAction("A","The Explosive Trap is quite expensive, so maybe having a few more Workmen would help you.",0.55,0.55,20000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","You need 500 units of each resource to be able to set this trap.",0.55,0.55,0,"BadPrincessHud/LumberjackPortrait",""))	
	
	a1 = CheckResourceUnitsAction(0,500)
	local a2 = CheckResourceUnitsAction(1,500)
	local a3 = CheckResourceUnitsAction(2,500)
	a2:AddStartActionNode(a3)
	a1:AddStartActionNode(a2)
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a1)

	AddActionNode(ShowMessageAction("A","Now, set the trap (pushing the first button or Q) close to the chest.",0.55,0.55,0,"BadPrincessHud/LumberjackPortrait",""))	
	a1 = CheckDistanceBetweenEntitiesAction("DamageTrap0","Chest",60)
	a1:AddEndActionNode(RemoveMessageAction("A"))
	AddActionNode(a1)
	
end

function bigWaveWarning()

	AddActionNode(ShowMessageAction("A","Our explorers have reported that a small battalion of Knights are approaching. It seems that some local bandits have join then to seize the opportunity of getting some loot",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","They will be here in 3 minutes, you have that time to get ready",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","Try new things: for example, the mud trap of the Ranger will slow down your enemies. Set them cleverly.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	enableAllAbilities()
	
	AddActionNode(ShowMessageAction("A","Also, I only told you about one secret power of the Princess... but she has 3 more!",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	

end

function bigWave()
	
	AddActionNode(ShowMessageAction("A","Well, it's time to get ready for the battle.",0.55,0.55, 20000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","This time, you will be at your own. I cannot take the risk of being spotted by one of those ruthless Knights.",0.55,0.55, 20000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","Good luck!",0.55,0.55, 20000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(BlockCameraAction(false))
	
	AddActionNode(TimerAction("FFFAFA00",180000))

	AddActionNode(DefendToWaveAction(3,1000))
	AddActionNode(DefendToWaveAction(4,1000))
	AddActionNode(DefendToWaveAction(5,1000))
	AddActionNode(DefendToWaveAction(6,1000))	
	AddActionNode(KillAllEnemiesAction())
	AddActionNode(IncrementsWavesCounter(1,3))
	
end

function assassinIntroduction()
	
	AddActionNode(ShowMessageAction("A","That was fun, right?",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","Sadly, this isn't over.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","Our spies (yes, we have spies, they were on sale...) has informed us that the reign of Cydonia has hired an Assassin to accompany the next battalion.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","They are the most dangerous enemy you will face, for they can take down our units very easily.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","In addition to that, they are stealth masters, and, while moving, they can become invisible for a moment.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","The good news are that they have no armour, so if any of your units get to spot and damage them, they will be defeated quickly..",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","And... do you remember the Ranger's Mud Trap I told you before?",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","It doesn't only slow down enemies, but also make visible any Assassin walking through it. As I said, set them cleverly",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","Well, they are coming. You have another 3 minutes to get ready.",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","For the Princess!!!",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	
	AddActionNode(TimerAction("FFFAFA00",180000))
	
end

function lastWave()

	AddActionNode(DefendToWaveAction(7,1000))
	AddActionNode(DefendToWaveAction(8,1000))
	AddActionNode(DefendToWaveAction(9,1000))
	AddActionNode(DefendToWaveAction(10,1000))	
	AddActionNode(KillAllEnemiesAction())
	AddActionNode(IncrementsWavesCounter(1,3))
	
end

function ending()
	
	AddActionNode(ShowMessageAction("A","Well lad, nice job!",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","I think there is nothing more I can teach you. You are now prepared to help the princess in her quest!",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	AddActionNode(ShowMessageAction("A","See ya!!",0.55,0.55,10000,"BadPrincessHud/LumberjackPortrait",""))	
	
end

function activateCampExample()
	
	AddActionNode(WaitTimeAction(10000))
	local mess = ActivateCamp(11)
	local a = SendMessageAction("Camp1",mess)
	AddActionNode(a)
	AddActionNode(WaitTimeAction(30000))
	local mess = ActivateCamp(12,10000)
	local a = SendMessageAction("Camp1",mess)
	AddActionNode(a)

	
end


function createWarriors(num)
	for variable = 1, num, 1 do
		AddActionNode(CreateEntityAction("WarriorGoblin","WellBase"))
	end	
end

function createWorkmen(num)
	for variable = 1, num, 1 do
		AddActionNode(CreateEntityAction("WorkmanGoblin","WellBase"))
	end	
end

function createRangers(num)
	for variable = 1, num, 1 do
		AddActionNode(CreateEntityAction("RangerGoblin","WellBase"))
	end	
end
