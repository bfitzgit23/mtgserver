local SpaceHelpers = require("utils.space_helpers")

imperialBrokerConvoHandler = conv_handler:new {}

function imperialBrokerConvoHandler:getInitialScreen(pPlayer, pNpc, pConvTemplate)
	local convoTemplate = LuaConversationTemplate(pConvTemplate)

	local faction = CreatureObject(pPlayer):getFaction()
	local playerFactionStatus = CreatureObject(pPlayer):getFactionStatus()
	local playerID = CreatureObject(pPlayer):getObjectID()
	local playerGender = CreatureObject(pPlayer):getGender()

	-- Player is Rebel Pilot
	if (SpaceHelpers:isRebelPilot(pPlayer)) then
		if (readData(playerID .. ":ImperialBroker:rebel:") == 1) then
			CreatureObject(pNpc):doAnimation("wave_on_dismissing")
			return convoTemplate:getScreen("get_lost_")
		else
			return convoTemplate:getScreen("rebel_player")
		end
	-- Player is a general pilot (not Imperial)
	elseif (SpaceHelpers:isPilot(pPlayer) and not SpaceHelpers:isImperialPilot(pPlayer)) then
		return convoTemplate:getScreen("no_business")
	end

	-- Check if player is an Imperial Pilot and part of Black Epsilon Squadron
	local isImperialPilot = SpaceHelpers:isImperialPilot(pPlayer)
	local isBlackEpsilonSquadron = SpaceHelpers:isBlackEpsilonSquadron(pPlayer)
	local hasShip = SpaceHelpers:hasCertifiedShip(pPlayer, true)

	-- These variables will now refer to the globally registered BlackEpsilonSquadronScreenplay and its quests
	local questOneStarted = SpaceHelpers:isSpaceQuestActive(pPlayer, BlackEpsilonSquadronScreenplay.QUEST_STRING_1.type, BlackEpsilonSquadronScreenplay.QUEST_STRING_1.name)
	local questTwoStarted = SpaceHelpers:isSpaceQuestActive(pPlayer, BlackEpsilonSquadronScreenplay.QUEST_STRING_2.type, BlackEpsilonSquadronScreenplay.QUEST_STRING_2.name)
	local questThreeStarted = SpaceHelpers:isSpaceQuestActive(pPlayer, BlackEpsilonSquadronScreenplay.QUEST_STRING_3.type, BlackEpsilonSquadronScreenplay.QUEST_STRING_3.name)
	local questFourStarted = SpaceHelpers:isSpaceQuestActive(pPlayer, BlackEpsilonSquadronScreenplay.QUEST_STRING_4.type, BlackEpsilonSquadronScreenplay.QUEST_STRING_4.name)

	local questOneComplete = SpaceHelpers:isSpaceQuestComplete(pPlayer, BlackEpsilonSquadronScreenplay.QUEST_STRING_1.type, BlackEpsilonSquadronScreenplay.QUEST_STRING_1.name) and SpaceHelpers:isSpaceQuestComplete(pPlayer, BlackEpsilonSquadronScreenplay.QUEST_STRING_1_SIDE.type, BlackEpsilonSquadronScreenplay.QUEST_STRING_1_SIDE.name)
	local questTwoComplete = SpaceHelpers:isSpaceQuestComplete(pPlayer, BlackEpsilonSquadronScreenplay.QUEST_STRING_2.type, BlackEpsilonSquadronScreenplay.QUEST_STRING_2.name)
	local questThreeComplete = SpaceHelpers:isSpaceQuestComplete(pPlayer, BlackEpsilonSquadronScreenplay.QUEST_STRING_3.type, BlackEpsilonSquadronScreenplay.QUEST_STRING_3.name) and SpaceHelpers:isSpaceQuestComplete(pPlayer, BlackEpsilonSquadronScreenplay.QUEST_STRING_3_SIDE.type, BlackEpsilonSquadronScreenplay.QUEST_STRING_3_SIDE.name)
	local questFourComplete = SpaceHelpers:isSpaceQuestComplete(pPlayer, BlackEpsilonSquadronScreenplay.QUEST_STRING_4.type, BlackEpsilonSquadronScreenplay.QUEST_STRING_4.name)

	local destroyDutyStarted = SpaceHelpers:isSpaceQuestActive(pPlayer, BlackEpsilonSquadronScreenplay.QUEST_STRING_DUTY_4_1.type, BlackEpsilonSquadronScreenplay.QUEST_STRING_DUTY_4_1.name)
	local escortDutyStarted = SpaceHelpers:isSpaceQuestActive(pPlayer, BlackEpsilonSquadronScreenplay.QUEST_STRING_DUTY_4_2.type, BlackEpsilonSquadronScreenplay.QUEST_STRING_DUTY_4_2.name)

	local destroyDutyComplete = SpaceHelpers:isSpaceQuestComplete(pPlayer, BlackEpsilonSquadronScreenplay.QUEST_STRING_DUTY_4_1.type, BlackEpsilonSquadronScreenplay.QUEST_STRING_DUTY_4_1.name)
	local escortDutyComplete = SpaceHelpers:isSpaceQuestComplete(pPlayer, BlackEpsilonSquadronScreenplay.QUEST_STRING_DUTY_4_2.type, BlackEpsilonSquadronScreenplay.QUEST_STRING_DUTY_4_2.name)

	-- If player is an Imperial Pilot and part of Black Epsilon Squadron
	if (isImperialPilot and isBlackEpsilonSquadron) then
        -- Check to ensure player has a starter ship or one they can use
        if (not hasShip and not questOneStarted) then
            return convoTemplate:getScreen("no_ship") -- This screen exists in imperial_broker_convo.lua now
        end
		-- Player has an active quest from the Broker
		if ((questTwoStarted and not questTwoComplete) or (questThreeStarted and not questThreeComplete) or (questFourStarted and not questFourComplete) or (destroyDutyStarted and not destroyDutyComplete) or (escortDutyStarted and not escortDutyComplete)) then
			return convoTemplate:getScreen("imperial_has_mission")
		-- Check if players have all the tier1 skill boxes, send them to next trainer.
		elseif (SpaceHelpers:hasCompletedPilotTier(pPlayer, "imperial_navy", 1)) then
			return convoTemplate:getScreen("imperial_completed_tier1")
		-- Player is a Black Epsilon pilot and has at least one of the Tier1 skill boxes
		elseif (SpaceHelpers:hasPilotTierSkill(pPlayer, "imperial_navy", 1)) then
			-- Check if the player can be trained in the remaining Tier1 Skills
			if (SpaceHelpers:hasExperienceForTraining(pPlayer, 1)) then
				return convoTemplate:getScreen("imperial_more_training")
			-- Offer Duty missions
			else
				CreatureObject(pPlayer):doAnimation("salute1")
				return convoTemplate:getScreen("imperial_duty_missions")
			end
		-- Player has finished 4 and has received the reward, but needs to accept training of first pilot skill
		elseif (questFourComplete and getQuestStatus(playerID .. BlackEpsilonSquadronScreenplay.QUEST_STRING_4.name .. ":reward") == "1") then
			return convoTemplate:getScreen("imperial_missions_complete")
		-- Player has completed quest 4 and needs reward
		elseif (questFourComplete and getQuestStatus(playerID .. BlackEpsilonSquadronScreenplay.QUEST_STRING_4.name .. ":reward") ~= "1") then
			-- Give player the reward and update that they received it
			setQuestStatus(playerID .. BlackEpsilonSquadronScreenplay.QUEST_STRING_4.name .. ":reward", 1)
			-- Grant Reward
			assassinate_naboo_imperial_tier1_4a:rewardPlayer(pPlayer)
			return convoTemplate:getScreen("imperial_missions_complete")
		-- Player has finished 3, has received the reward and needs to start quest 4
		elseif (questThreeComplete and not questFourStarted and getQuestStatus(playerID .. BlackEpsilonSquadronScreenplay.QUEST_STRING_3.name .. ":reward") == "1") then
			return convoTemplate:getScreen("imperial_excellent_work3")
		-- Player has completed quest 3 and needs reward
		elseif (questThreeComplete and getQuestStatus(playerID .. BlackEpsilonSquadronScreenplay.QUEST_STRING_3.name .. ":reward") ~= "1") then
			-- Give player the reward and update that they received it
			setQuestStatus(playerID .. BlackEpsilonSquadronScreenplay.QUEST_STRING_3.name .. ":reward", 1)
			-- Grant Reward
			patrol_naboo_imperial_3:rewardPlayer(pPlayer)
			return convoTemplate:getScreen("imperial_excellent_work3")
		-- Player has finished 2, has received the reward and needs to start quest 3
		elseif (questTwoComplete and not questThreeStarted and getQuestStatus(playerID .. BlackEpsilonSquadronScreenplay.QUEST_STRING_2.name .. ":reward") == "1") then
			return convoTemplate:getScreen("imperial_excellent_work2")
		-- Player has completed quest 2 and needs reward
		elseif (questTwoComplete and getQuestStatus(playerID .. BlackEpsilonSquadronScreenplay.QUEST_STRING_2.name .. ":reward") ~= "1") then
			-- Give player the reward and update that they received it
			setQuestStatus(playerID .. BlackEpsilonSquadronScreenplay.QUEST_STRING_2.name .. ":reward", 1)
			-- Grant Reward
			destroy_naboo_imperial_2:rewardPlayer(pPlayer)
			return convoTemplate:getScreen("imperial_excellent_work2")
		-- Player has finished quest 1, has received the reward and needs to start quest 2
		elseif (questOneComplete and not questTwoStarted and getQuestStatus(playerID .. BlackEpsilonSquadronScreenplay.QUEST_STRING_1.name .. ":reward") == "1") then
			return convoTemplate:getScreen("imperial_excellent_work")
		-- Player has finished quest 1 and needs reward
		elseif (questOneComplete and getQuestStatus(playerID .. BlackEpsilonSquadronScreenplay.QUEST_STRING_1.name .. ":reward") ~= 1) then
			-- Give player the reward and update that they received it
			setQuestStatus(playerID .. BlackEpsilonSquadronScreenplay.QUEST_STRING_1.name .. ":reward", 1)
			-- Grant Reward
			patrol_naboo_imperial_1:rewardPlayer(pPlayer)
			return convoTemplate:getScreen("imperial_excellent_work")
		-- Player has first quest active, the mission giver will offer assistance
		elseif (questOneStarted and not questOneComplete) then
			return convoTemplate:getScreen("imperial_first_assignment")
		-- Player has failed or aborted the first quest, or ready to start the first quest
		elseif (not questOneComplete) then
			return convoTemplate:getScreen("imperial_yes_im_ready")
		end

	-- Player is Imperial but not yet assigned to a squadron or Black Epsilon specifically
	elseif (isImperialPilot and not isBlackEpsilonSquadron) then
		CreatureObject(pNpc):doAnimation("salute2")
		return convoTemplate:getScreen("hello_imperial_pilot")

	-- If the player is Imperial Faction but not a pilot yet, or on leave
	elseif (faction == FACTIONIMPERIAL and playerFactionStatus > ONLEAVE) then
		CreatureObject(pNpc):doAnimation("salute1")
		CreatureObject(pPlayer):doAnimation("salute1")

		-- Male imperial
		if (playerGender == 0) then
			return convoTemplate:getScreen("greet_male_imp")
		-- Female imperial
		else
			return convoTemplate:getScreen("greet_female_imp")
		end
	end

	-- Already conversed with broker (general neutral, or non-pilot Imperial who already saw recruit message)
	if (readData(playerID .. ":ImperialBroker:") == 1) then
		CreatureObject(pNpc):doAnimation("shrug_hands")
		-- Male
		if (playerGender == 0) then
			return convoTemplate:getScreen("something_else_male")
		-- Female
		else
			return convoTemplate:getScreen("something_else_female")
		end
	-- Neutral Female
	elseif (faction == FACTIONNEUTRAL and playerGender == 1) then
		CreatureObject(pNpc):doAnimation("point_away")
		return convoTemplate:getScreen("neutral_female")
	-- Neutral Wookiee
	elseif (CreatureObject(pPlayer):getSpecies() == SPECIES_WOOKIEE) then
		CreatureObject(pNpc):doAnimation("belly_laugh")
		return convoTemplate:getScreen("neutral_wookiee")
	end

	-- Default for fresh neutral players
	return convoTemplate:getScreen("greet_neutral")
end

function imperialBrokerConvoHandler:runScreenHandlers(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
	if (pPlayer == nil or pConvScreen == nil) then
		return
	end

	local screen = LuaConversationScreen(pConvScreen)
	local screenID = screen:getScreenID()

	local pClonedScreen = screen:cloneScreen()
	local clonedConversation = LuaConversationScreen(pClonedScreen)

	-- Set player as conversation target
	clonedConversation:setDialogTextTU(CreatureObject(pPlayer):getFirstName())

	local pGhost = CreatureObject(pPlayer):getPlayerObject()

	if (pGhost == nil) then
		return pClonedScreen
	end

	local ghost = LuaPlayerObject(pGhost)

	if (ghost == nil) then
		return pClonedScreen
	end

	local playerGender = CreatureObject(pPlayer):getGender()

	-- Handle Imperial Pilot Recruitment / Joining Black Epsilon
	if (screenID == "yes_neutral_recruit" or screenID == "yes_recruiting_male" or screenID == "yes_recruiting_female") then
		CreatureObject(pNpc):doAnimation("explain")
		SpaceHelpers:grantNovicePilot(pPlayer, "imperialPilot")
		SpaceHelpers:setSquadronType(pPlayer, BLACK_EPSILON_SQUADRON)
		PlayerObject(pGhost):incrementPilotTier()

		if (not SpaceHelpers:hasCertifiedShip(pPlayer, true)) then
			grantStarterShip(pPlayer, "imperial")
		end
		return pClonedScreen
	-- Handle initial recruitment rejection
	elseif (screenID == "no_just_checking_male" or screenID == "no_just_checking_female" or screenID == "no_just_checking") then
		CreatureObject(pNpc):doAnimation("salute1")
	elseif (screenID == "never_mind" or screenID == "no_never_mind") then
		CreatureObject(pNpc):doAnimation("salute2")
	-- Handling for existing Imperial Pilots not in Black Epsilon, allowing them to join
	elseif (screenID == "join_black_epsilon_squadron") then
		CreatureObject(pNpc):doAnimation("nod_head_once")
		SpaceHelpers:setSquadronType(pPlayer, BLACK_EPSILON_SQUADRON)
		return pClonedScreen

	-- Imperial Training (dynamic options are added here)
	elseif (screenID == "imperial_more_training") then
		local skillManager = LuaSkillManager()

		-- The text for these options is defined directly in the conversation .lua file, not here.
		if (not CreatureObject(pPlayer):hasSkill("pilot_imperial_navy_starships_01") and skillManager:fulfillsSkillPrerequisitesAndXp(pPlayer, "pilot_imperial_navy_starships_01")) then
			clonedConversation:addOption("@conversation/imperial_broker:s_imperial_fighters_train", "train_player_imperial_fighters")
		end
		if (not CreatureObject(pPlayer):hasSkill("pilot_imperial_navy_weapons_01") and skillManager:fulfillsSkillPrerequisitesAndXp(pPlayer, "pilot_imperial_navy_weapons_01")) then
			clonedConversation:addOption("@conversation/imperial_broker:s_imperial_component_train", "train_player_imperial_component")
		end
		if (not CreatureObject(pPlayer):hasSkill("pilot_imperial_navy_procedures_01") and skillManager:fulfillsSkillPrerequisitesAndXp(pPlayer, "pilot_imperial_navy_procedures_01")) then
			clonedConversation:addOption("@conversation/imperial_broker:s_imperial_basics_train", "train_player_imperial_basics")
		end
		if (not CreatureObject(pPlayer):hasSkill("pilot_imperial_navy_droid_01") and skillManager:fulfillsSkillPrerequisitesAndXp(pPlayer, "pilot_imperial_navy_droid_01")) then
			clonedConversation:addOption("@conversation/imperial_broker:s_imperial_droid_train", "train_player_imperial_droid")
		end
        return pClonedScreen
	-- Handle Skill box granting for Imperial
	elseif (string.find(screenID, "train_player_imperial_")) then
		local skillManager = LuaSkillManager()
		local deductExperience = (string.find(screenID, "_free") == nil)
		screenID = string.gsub(screenID, "_free", "")

		if (screenID == "train_player_imperial_droid") then
			if (not deductExperience or skillManager:fulfillsSkillPrerequisitesAndXp(pPlayer, "pilot_imperial_navy_droid_01")) then
				SpaceHelpers:grantSpaceSkill(pPlayer, "pilot_imperial_navy_droid_01", deductExperience)
			end
		elseif (screenID == "train_player_imperial_basics") then
			if (not deductExperience or skillManager:fulfillsSkillPrerequisitesAndXp(pPlayer, "pilot_imperial_navy_procedures_01")) then
				SpaceHelpers:grantSpaceSkill(pPlayer, "pilot_imperial_navy_procedures_01", deductExperience)
			end
		elseif (screenID == "train_player_imperial_fighters") then
			if (not deductExperience or skillManager:fulfillsSkillPrerequisitesAndXp(pPlayer, "pilot_imperial_navy_starships_01")) then
				SpaceHelpers:grantSpaceSkill(pPlayer, "pilot_imperial_navy_starships_01", deductExperience)
			end
		elseif (screenID == "train_player_imperial_component") then
			if (not deductExperience or skillManager:fulfillsSkillPrerequisitesAndXp(pPlayer, "pilot_imperial_navy_weapons_01")) then
				SpaceHelpers:grantSpaceSkill(pPlayer, "pilot_imperial_navy_weapons_01", deductExperience)
			end
		end
		return pClonedScreen

	-- Missions - Calling the Black Epsilon quest start functions
	elseif (screenID == "imperial_first_assignment_start") then
		patrol_naboo_imperial_1:startQuest(pPlayer, pNpc)
        return pClonedScreen
	elseif (screenID == "imperial_train_me2") then
		destroy_naboo_imperial_2:startQuest(pPlayer, pNpc)
        return pClonedScreen
	elseif (screenID == "imperial_train_me3") then
		patrol_naboo_imperial_3:startQuest(pPlayer, pNpc)
        return pClonedScreen
	elseif (screenID == "imperial_train_me4") then
		assassinate_naboo_imperial_tier1_4a:startQuest(pPlayer, pNpc)
        return pClonedScreen
	elseif (screenID == "imperial_destroy_duty") then
		destroy_duty_naboo_imperial_6:startQuest(pPlayer, pNpc)
        return pClonedScreen
	elseif (screenID == "imperial_escort_duty") then
		escort_duty_naboo_imperial_7:startQuest(pPlayer, pNpc)
        return pClonedScreen

	-- Original Imperial Broker logic (where options link to pre-defined screens)
	elseif (screenID == "more_black_epsilon_male" or screenID == "more_black_epsilon_female" or screenID == "more_black_epsilon_neutral") then
		CreatureObject(pNpc):doAnimation("explain")
		SpaceHelpers:addBlackEpsilonSquadWaypoint(pPlayer)
		imperialBrokerConvoHandler:setBrokerStatus(pPlayer)
	elseif (screenID == "storm_squadron") then
		CreatureObject(pNpc):doAnimation("pose_proudly")
	elseif (screenID == "not_interested_storm_male" or screenID == "not_interested_storm_female" or screenID == "storm_squadron_neutral_no_int") then
		CreatureObject(pNpc):doAnimation("shrug_hands")
	elseif (screenID == "storm_more_male" or screenID == "storm_more_female" or screenID == "more_storm_neutral") then
		CreatureObject(pNpc):doAnimation("explain")
		SpaceHelpers:addStormSquadWaypoint(pPlayer)
		imperialBrokerConvoHandler:setBrokerStatus(pPlayer)
	elseif (screenID == "not_interested_inquisition_male" or screenID == "not_interested_inquisition_female") then
		CreatureObject(pNpc):doAnimation("sigh_deeply")
	elseif (screenID == "what_inquisition_male" or screenID == "what_inquisition_female" or screenID == "more_inquisition_neutral") then
		CreatureObject(pNpc):doAnimation("manipulate_medium")
		SpaceHelpers:addImperialInquisitionSquadWaypoint(pPlayer)
		imperialBrokerConvoHandler:setBrokerStatus(pPlayer)
	elseif (screenID == "rebel_player") then
		-- Options for rebel player are defined in the .lua convo file directly
	elseif (screenID == "stop_bothering" or screenID == "pilot_wookiee" or screenID == "pilot_rebel" or screenID == "pilot_female") then
		CreatureObject(pNpc):doAnimation("wave_on_dismissing")
		imperialBrokerConvoHandler:setRebelBrokerStatus(pPlayer)
	-- Player is Imperial Pilot - Handle retirement
	elseif (screenID == "request_retire") then
		CreatureObject(pNpc):doAnimation("point_accusingly")
	elseif (screenID == "want_retirement") then
		CreatureObject(pNpc):doAnimation("shake_head_no")
	elseif (screenID == "confirm_retirement") then
		CreatureObject(pNpc):doAnimation("dismiss")
		SpaceHelpers:surrenderPilot(pPlayer)
		BlackEpsilonSquadronScreenplay:resetImperialQuests(pPlayer) -- Reset Imperial quests upon retirement
	end

	return pClonedScreen
end

function imperialBrokerConvoHandler:setBrokerStatus(pPlayer)
	if (pPlayer == nil) then
		return
	end

	local playerID = CreatureObject(pPlayer):getObjectID()

	writeData(playerID .. ":ImperialBroker:", 1)
end

function imperialBrokerConvoHandler:setRebelBrokerStatus(pPlayer)
	if (pPlayer == nil) then
		return
	end

	local playerID = CreatureObject(pPlayer):getObjectID()

	writeData(playerID .. ":ImperialBroker:rebel:", 1)
end