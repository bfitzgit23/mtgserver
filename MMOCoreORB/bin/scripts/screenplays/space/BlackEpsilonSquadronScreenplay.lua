local Logger = require("utils.logger")
local SpaceHelpers = require("utils.space_helpers") -- Ensure this is required if not already

--[[

	Black Epsilon Squadron Quests

]]

-- Imperial Broker Main Quests (equivalent to Rhea's main quests)

patrol_naboo_imperial_1 = SpacePatrolScreenplay:new {
	className = "patrol_naboo_imperial_1",

	questName = "naboo_imperial_1",
	questType = "patrol",

	questZone = "space_naboo",

	creditReward = 150, -- Slightly higher reward for Imperial quests

	sideQuest = true,
	sideQuestType = "destroy_surpriseattack",
	sideQuestStart = 2, -- Patrol Point Number
	sideQuestDelay = 25, -- Time in seconds to wait to trigger side quest

	patrolPoints = {
		{zoneName = "space_naboo", x = 5000, z = 0, y = 1000, patrolNumber = 1, radius = 200},
		{zoneName = "space_naboo", x = 6000, z = 500, y = 2000, patrolNumber = 2, radius = 200},
		{zoneName = "space_naboo", x = 7000, z = 1000, y = 3000, patrolNumber = 3, radius = 200},
	},
}

registerScreenPlay("patrol_naboo_imperial_1", true)

destroy_surpriseattack_naboo_imperial_1 = SpaceSurpriseAttackScreenplay:new {
	className = "destroy_surpriseattack_naboo_imperial_1",

	questName = "naboo_imperial_1",
	questType = "destroy_surpriseattack",

	questZone = "space_naboo",

	sideQuest = false,
	sideQuestType = "",

	parentQuest = "patrol_naboo_imperial_1",
	parentQuestType = "patrol",

	surpriseAttackShips = {
		zone = "space_naboo",
		spawns = {{count = 1, shipName = "rebel_fighter_s02_tier1"}, {count = 3, shipName = "rebel_fighter_s01_tier1"}}, -- Enemies are rebels
		total = 4,
	},
}

registerScreenPlay("destroy_surpriseattack_naboo_imperial_1", true)

destroy_naboo_imperial_2 = SpaceDestroyScreenplay:new {
	className = "destroy_naboo_imperial_2",

	questName = "naboo_imperial_2",
	questType = "destroy",

	questZone = "space_naboo",

	creditReward = 250,

	sideQuest = false,
	sideQuestType = "",

	killsRequired = 5,

	shipLocations = { -- Adjust coordinates for Naboo space
		{name = "rebel_outpost_1", x = 5500, z = 1500, y = 1000},
		{name = "rebel_outpost_2", x = 6500, z = 2000, y = 2500},
		{name = "rebel_outpost_3", x = 7500, z = 2500, y = 3000},
	},

	shipTypes = { -- Rebel ship types
		"rebel_ace_s04_tier1", "rebel_ace_s04_tier2", "rebel_aggressor_tier1", "rebel_aggressor_tier2", "rebel_bomber_ace_s04_tier1", "rebel_bomber_ace_s04_tier2",
		"rebel_bomber_s01_tier1", "rebel_bomber_s01_tier2", "rebel_bomber_s02_tier1", "rebel_bomber_s02_tier2", "rebel_bomber_s03_tier1", "rebel_bomber_s03_tier2",
		"rebel_fighter_s01_tier1", "rebel_fighter_s01_tier2", "rebel_fighter_s02_tier1", "rebel_fighter_s02_tier1", "rebel_fighter_s03_tier1", "rebel_fighter_s03_tier2",
	},
}

registerScreenPlay("destroy_naboo_imperial_2", true)

patrol_naboo_imperial_3 = SpacePatrolScreenplay:new {
	className = "patrol_naboo_imperial_3",

	questName = "naboo_imperial_3",
	questType = "patrol",

	questZone = "space_naboo",

	creditReward = 600,
	itemReward = {
		{species = {-1}, item = "object/tangible/wearables/armor/imperial_tie_pilot/armor_imperial_tie_pilot_s02_boots.iff"}, -- Example Imperial item
	},

	sideQuest = true,
	sideQuestType = "escort",
	sideQuestStart = 2,
	sideQuestDelay = 30, -- Time in seconds to wait to trigger side quest

	patrolPoints = {
		{zoneName = "space_naboo", x = 4000, z = -1000, y = -500, patrolNumber = 1, radius = 200},
		{zoneName = "space_naboo", x = 3000, z = -1500, y = -1000, patrolNumber = 2, radius = 200},
		{zoneName = "space_naboo", x = 2000, z = -2000, y = -1500, patrolNumber = 3, radius = 200},
		{zoneName = "space_naboo", x = 1000, z = -2500, y = -2000, patrolNumber = 4, radius = 200},
	},
}

registerScreenPlay("patrol_naboo_imperial_3", true)

escort_naboo_imperial_3 = SpaceEscortScreenplay:new {
	className = "escort_naboo_imperial_3",

	questName = "naboo_imperial_3",
	questType = "escort",

	questZone = "space_naboo",

	sideQuest = false,
	sideQuestType = "",

	parentQuest = "patrol_naboo_imperial_3",
	parentQuestType = "patrol",

	escortShip = "imperial_shuttle_tier1", -- Imperial shuttle to escort

	escortPoints = {
		{name = "imperial_escort_1", zoneName = "space_naboo", x = 4500, z = -1200, y = -700, escortNumber = 1, radius = 300},
		{name = "imperial_escort_2", zoneName = "space_naboo", x = 3500, z = -1700, y = -1200, escortNumber = 2, radius = 300},
		{name = "imperial_escort_3", zoneName = "space_naboo", x = 2500, z = -2200, y = -1700, escortNumber = 3, radius = 300},
		{name = "imperial_escort_4", zoneName = "space_naboo", x = 1500, z = -2700, y = -2200, escortNumber = 4, radius = 300},
	},

	attackDelay = 100, -- In Seconds
	attackShips = {"rebel_fighter_s02_tier1", "rebel_fighter_s03_tier1", "rebel_fighter_s02_tier1"}, -- Enemies are rebels
}

registerScreenPlay("escort_naboo_imperial_3", true)

assassinate_naboo_imperial_tier1_4a = SpaceAssassinateScreenplay:new {
	className = "assassinate_naboo_imperial_tier1_4a",

	questType = "assassinate",
	questName = "naboo_imperial_tier1_4a",

	questZone = "space_naboo",

	creditReward = 1200,
	itemReward = {
		{species = {-1}, item = "object/tangible/ship/components/weapon/wpn_mission_reward_imperial_blaster.iff"}, -- Example Imperial weapon
	},

	sideQuest = false,
	sideQuestType = "",

	arrivalDelay = 7, -- Seconds
	failTimer = 25, -- Minutes

	assassinateSpawns = {
		target = "rebel_ace_s04_tier2", -- Rebel ace target
		escorts = {"rebel_fighter_s01_tier1", "rebel_fighter_s01_tier1", "rebel_fighter_s01_tier1"},
	},

	targetPatrols = { -- Adjust coordinates for Naboo space
		{name = "assassinate_naboo_1", x = 7000, z = 3000, y = 4000},
		{name = "assassinate_naboo_2", x = 8000, z = 3500, y = 4500},
		{name = "assassinate_naboo_3", x = 5000, z = 2000, y = 2500},
	},
}

registerScreenPlay("assassinate_naboo_imperial_tier1_4a", true)

-- Imperial Broker Duty Quests (equivalent to Rhea's duty quests)

destroy_duty_naboo_imperial_6 = SpaceDutyDestroyScreenplay:new {
	className = "destroy_duty_naboo_imperial_6",

	questName = "naboo_imperial_6",
	questType = "destroy_duty",

	questZone = "space_naboo",

	creditReward = 120,

	sideQuest = false,
	sideQuestType = "",

	killsRequired = 5,

	shipLocations = { -- Adjust coordinates for Naboo space
		{name = "rebel_hideout_1", x = 500, z = -500, y = -1000},
		{name = "rebel_hideout_2", x = 1000, z = -1000, y = -1500},
		{name = "rebel_hideout_3", x = 1500, z = -1500, y = -2000},
	},

	shipTypes = { -- Rebel ship types
		"rebel_fighter_s01_tier1", "rebel_fighter_s02_tier1", "rebel_bomber_s01_tier1", "rebel_aggressor_tier1",
	},
}

registerScreenPlay("destroy_duty_naboo_imperial_6", true)

escort_duty_naboo_imperial_7 = SpaceDutyEscortScreenplay:new {
	className = "escort_duty_naboo_imperial_7",

	questName = "naboo_imperial_7",
	questType = "escort_duty",

	questZone = "space_naboo",

	creditReward = 1100,

	itemReward = {
		-- No specific item reward for duty missions in original, but could add here.
	},

	sideQuest = false,
	sideQuestType = "",

	escortShips = {"imperial_transport_tier1", "imperial_tie_fighter_s01_tier1"}, -- Imperial ships to escort

	escortPoints = {
		{name = "imperial_duty_escort_1", zoneName = "space_naboo", x = 2000, z = 500, y = 1000, escortNumber = 1, radius = 300},
		{name = "imperial_duty_escort_2", zoneName = "space_naboo", x = 2500, z = 1000, y = 1500, escortNumber = 2, radius = 300},
		{name = "imperial_duty_escort_3", zoneName = "space_naboo", x = 3000, z = 1500, y = 2000, escortNumber = 3, radius = 300},
	},

	attackDelay = 110, -- In Seconds

	totalAttackGroups = 2,

	attackGroups = {
		{"rebel_fighter_s01_tier1", "rebel_fighter_s02_tier1"},
		{"rebel_bomber_tier1", "rebel_fighter_s03_tier1"},
	},

	creditKillBonus = 120,
}

registerScreenPlay("escort_duty_naboo_imperial_7", true)


--[[

	BlackEpsilonSquadronScreenplay

]]

BlackEpsilonSquadronScreenplay = ScreenPlay:new {
	screenplayName = "BlackEpsilonSquadronScreenplay",

	DEBUG_BLACK_EPSILON = false,

	QUEST_STRING_1 = {type = "patrol", name = "naboo_imperial_1"},
	QUEST_STRING_1_SIDE = {type = "destroy_surpriseattack", name = "naboo_imperial_1"},
	QUEST_STRING_2 = {type = "destroy", name = "naboo_imperial_2"},
	QUEST_STRING_3 = {type = "patrol", name = "naboo_imperial_3"},
	QUEST_STRING_3_SIDE = {type = "escort", name = "naboo_imperial_3"},
	QUEST_STRING_4 = {type = "assassinate", name = "naboo_imperial_tier1_4a"},
	QUEST_STRING_DUTY_4_1 = {type = "destroy_duty", name = "naboo_imperial_6"},
	QUEST_STRING_DUTY_4_2 = {type = "escort_duty", name = "naboo_imperial_7"},
}

registerScreenPlay("BlackEpsilonSquadronScreenplay", false)

function BlackEpsilonSquadronScreenplay:start()
end

function BlackEpsilonSquadronScreenplay:resetImperialQuests(pPlayer)
	if (pPlayer == nil) then
		return
	end

	-- Fail and clear all active Imperial quests for this squadron
	patrol_naboo_imperial_1:failQuest(pPlayer, "false")
	destroy_surpriseattack_naboo_imperial_1:failQuest(pPlayer, "false")
	destroy_naboo_imperial_2:failQuest(pPlayer, "false")
	patrol_naboo_imperial_3:failQuest(pPlayer, "false")
	escort_naboo_imperial_3:failQuest(pPlayer, "false")
	assassinate_naboo_imperial_tier1_4a:failQuest(pPlayer, "false")

	SpaceHelpers:failSpaceQuest(pPlayer, self.QUEST_STRING_1.type, self.QUEST_STRING_1.name, false)
	SpaceHelpers:clearSpaceQuest(pPlayer, self.QUEST_STRING_1.type, self.QUEST_STRING_1.name, false)

	SpaceHelpers:failSpaceQuest(pPlayer, self.QUEST_STRING_1_SIDE.type, self.QUEST_STRING_1_SIDE.name, false)
	SpaceHelpers:clearSpaceQuest(pPlayer, self.QUEST_STRING_1_SIDE.type, self.QUEST_STRING_1_SIDE.name, false)

	SpaceHelpers:failSpaceQuest(pPlayer, self.QUEST_STRING_2.type, self.QUEST_STRING_2.name, false)
	SpaceHelpers:clearSpaceQuest(pPlayer, self.QUEST_STRING_2.type, self.QUEST_STRING_2.name, false)

	SpaceHelpers:failSpaceQuest(pPlayer, self.QUEST_STRING_3.type, self.QUEST_STRING_3.name, false)
	SpaceHelpers:clearSpaceQuest(pPlayer, self.QUEST_STRING_3.type, self.QUEST_STRING_3.name, false)

	SpaceHelpers:failSpaceQuest(pPlayer, self.QUEST_STRING_3_SIDE.type, self.QUEST_STRING_3_SIDE.name, false)
	SpaceHelpers:clearSpaceQuest(pPlayer, self.QUEST_STRING_3_SIDE.type, self.QUEST_STRING_3_SIDE.name, false)

	SpaceHelpers:failSpaceQuest(pPlayer, self.QUEST_STRING_4.type, self.QUEST_STRING_4.name, false)
	SpaceHelpers:clearSpaceQuest(pPlayer, self.QUEST_STRING_4.type, self.QUEST_STRING_4.name, false)

	local playerID = SceneObject(pPlayer):getObjectID()

	removeQuestStatus(playerID .. BlackEpsilonSquadronScreenplay.QUEST_STRING_1.name .. ":reward")
	removeQuestStatus(playerID .. BlackEpsilonSquadronScreenplay.QUEST_STRING_2.name .. ":reward")
	removeQuestStatus(playerID .. BlackEpsilonSquadronScreenplay.QUEST_STRING_3.name .. ":reward")
	removeQuestStatus(playerID .. BlackEpsilonSquadronScreenplay.QUEST_STRING_4.name .. ":reward")

	-- It is crucial to also clear any active duty missions, if they are separate from the main line quests
	SpaceHelpers:failSpaceQuest(pPlayer, self.QUEST_STRING_DUTY_4_1.type, self.QUEST_STRING_DUTY_4_1.name, false)
	SpaceHelpers:clearSpaceQuest(pPlayer, self.QUEST_STRING_DUTY_4_1.type, self.QUEST_STRING_DUTY_4_1.name, false)
	SpaceHelpers:failSpaceQuest(pPlayer, self.QUEST_STRING_DUTY_4_2.type, self.QUEST_STRING_DUTY_4_2.name, false)
	SpaceHelpers:clearSpaceQuest(pPlayer, self.QUEST_STRING_DUTY_4_2.type, self.QUEST_STRING_DUTY_4_2.name, false)
end