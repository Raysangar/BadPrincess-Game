WorkmanGoblinAnimation = {
	Walk = {},
	Attack1 = 	
	{
		{
				bone = "Bone_R_Hand_End",
				mesh = "sword.mesh",
				name = "sword1"
		}

	},
	--[[
	CarryChest = {
		{
				bone = "Bone_R_Hand_End",
				mesh = "chest_rope.mesh",
				name = "chestCarried1"
		}
	},

	Idle3 = 	
	{
		{
				bone = "Bone_R_Hand_End",
				mesh = "chest_rope.mesh",
				name = "chestCarried2"
		}

	},
]]--	
	Gather = {
		{
				bone = "Bone_R_Hand_End",
				mesh = "axe.mesh",
				name = "axeGather"
		}
	},
	Death = {},
}

KnightAnimation = {
	Idle = 	
	{
		{
				bone = "r_weapon",
				mesh = "swordKnight.mesh",
				name = "sword_idle"
		},
		{
				bone = "l_shield",
				mesh = "shield.mesh",
				name = "shield_idle"
		}

	},
	
	Walk = 	
	{
		{
				bone = "r_weapon",
				mesh = "swordKnight.mesh",
				name = "sword_walk"
		},
		{
				bone = "l_shield",
				mesh = "shield.mesh",
				name = "shield_walk"
		}

	},
	
	Attack = 	
	{
		{
				bone = "r_weapon",
				mesh = "swordKnight.mesh",
				name = "sword_attack"
		},
		{
				bone = "l_shield",
				mesh = "shield.mesh",
				name = "shield_attack"
		}

	},
	
	Death = 	
	{
		{
				bone = "r_weapon",
				mesh = "swordKnight.mesh",
				name = "sword_death"
		},
		{
				bone = "l_shield",
				mesh = "shield.mesh",
				name = "shield_death"
		}

	}
}


AssassinAnimation = {
	Idle = 	
	{
		{
				bone = "r_weapon",
				mesh = "dagger.mesh",
				name = "dagger_idle"
		}
	},
	
	Walk = 	
	{
		{
				bone = "r_weapon",
				mesh = "dagger.mesh",
				name = "dagger_walk"
		}
	},
	
	Attack = 	
	{
		{
				bone = "r_weapon",
				mesh = "dagger.mesh",
				name = "dagger_attack"
		}
	},
	
	Death = 	
	{
		{
				bone = "r_weapon",
				mesh = "dagger.mesh",
				name = "dagger_death"
		}
	},
	
	Stunned = 	
	{
		{
				bone = "r_weapon",
				mesh = "dagger.mesh",
				name = "dagger_stunned"
		}
	}
}

WorkmanGoblinMenuAnimation = {
	Walk = {},
	Gather = {
		{
				bone = "Bone_R_Hand_End",
				mesh = "axe.mesh",
				name = "axeGather"
		}
	},
	Death = {},
}
WarriorGoblinAnimation = {
	Attack1 = 	
	{
		{
				bone = "Bone_R_Hand_End",
				mesh = "sword.mesh",
				name = "sword1"
		}

	}
}

RangerGoblinAnimation = {
	RangerAttack = 	
	{
		{
				bone = "Bone_L_Hand",
				mesh = "bow.mesh",
				name = "bow"
		}

	}
}
Knight_TestAnimation = {
	Idle = 	
	{
		{
				bone = "r_weapon",
				mesh = "sword_test.mesh",
				name = "sword1"
		},
		{
				bone = "l_shield",
				mesh = "shield_test.mesh",
				name = "shield"
		}

	}
}

print("load Animation inventary completed")