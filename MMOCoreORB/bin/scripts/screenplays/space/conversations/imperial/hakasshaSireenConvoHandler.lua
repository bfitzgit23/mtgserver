local SpaceHelpers = require("utils.space_helpers")
-- BlackEpsilonSquadronScreenplay is accessed globally due to registerScreenPlay
-- SpaceDutyDestroyScreenplay and SpaceDutyEscortScreenplay are accessed globally due to registerScreenPlay

hakasshaSireenConvoHandler = conv_handler:new {}

function hakasshaSireenConvoHandler:getInitialScreen(pPlayer, pNpc, pConvTemplate)
    local convoTemplate = LuaConversationTemplate(pConvTemplate)

    -- Ensure the player is an Imperial pilot and in the Black Epsilon Squadron
    if (not SpaceHelpers:isImperialPilot(pPlayer) or not SpaceHelpers:isBlackEpsilonSquadron(pPlayer)) then
        return convoTemplate:getScreen("no_business")
    end

    local isTier1Complete = SpaceHelpers:hasCompletedPilotTier(pPlayer, "imperial_navy", 1)
    local hasAnyTier1Skill = SpaceHelpers:hasPilotTierSkill(pPlayer, "imperial_navy", 1)

    -- Player has completed all Tier 1 skills
    if (isTier1Complete) then
        return convoTemplate:getScreen("completed_all_tier1")
    -- Player has some Tier 1 skills, but not all (offer remaining training)
    elseif (hasAnyTier1Skill) then
        return convoTemplate:getScreen("offer_tier1_training")
    end

    -- Default greeting for new Black Epsilon recruits who need initial Tier 1 training
    return convoTemplate:getScreen("greet_imperial")
end

function hakasshaSireenConvoHandler:runScreenHandlers(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
    local screen = LuaConversationScreen(pConvScreen)
    local screenID = screen:getScreenID()
    local clonedConversation = LuaConversationScreen(screen:cloneScreen())
    local skillManager = LuaSkillManager()

    -- Dynamically add options for missing Tier 1 Imperial skills on "offer_tier1_training" screen
    if (screenID == "offer_tier1_training") then
        -- Remove existing options to replace with dynamic ones, or ensure only default "goodbye" is there
        -- You might need to clear existing options if the convo template defines them statically here.
        -- clonedConversation:clearOptions() -- Use if you want to remove ALL pre-defined options for this screen

        -- Add options for skills not yet learned and requirements met
        if (not CreatureObject(pPlayer):hasSkill("pilot_imperial_navy_starships_01") and skillManager:fulfillsSkillPrerequisitesAndXp(pPlayer, "pilot_imperial_navy_starships_01")) then
            clonedConversation:addOption("Train me in Imperial Starships basics.", "train_imperial_fighters_s01_tier1")
        end
        if (not CreatureObject(pPlayer):hasSkill("pilot_imperial_navy_weapons_01") and skillManager:fulfillsSkillPrerequisitesAndXp(pPlayer, "pilot_imperial_navy_weapons_01")) then
            clonedConversation:addOption("Train me in Imperial Weapons basics.", "train_imperial_weapons_s01_tier1")
        end
        if (not CreatureObject(pPlayer):hasSkill("pilot_imperial_navy_procedures_01") and skillManager:fulfillsSkillPrerequisitesAndXp(pPlayer, "pilot_imperial_navy_procedures_01")) then
            clonedConversation:addOption("Train me in Imperial Procedures basics.", "train_imperial_procedures_s01_tier1")
        end
        if (not CreatureObject(pPlayer):hasSkill("pilot_imperial_navy_droid_01") and skillManager:fulfillsSkillPrerequisitesAndXp(pPlayer, "pilot_imperial_navy_droid_01")) then
            clonedConversation:addOption("Train me in Imperial Droid basics.", "train_imperial_droid_s01_tier1")
        end
        -- Add a fallback/goodbye option if no training options are available
        if (clonedConversation:getOptionCount() == 0) then
            clonedConversation:addOption("I've learned all I can from you for now.", "goodbye")
        else
            -- If there are training options, also allow a general goodbye
            clonedConversation:addOption("That's enough for now.", "goodbye")
        end
    -- Skill granting logic based on selected option
    elseif (screenID == "train_imperial_fighters_s01_tier1") then
        SpaceHelpers:grantSpaceSkill(pPlayer, "pilot_imperial_navy_starships_01", true)
    elseif (screenID == "train_imperial_weapons_s01_tier1") then
        SpaceHelpers:grantSpaceSkill(pPlayer, "pilot_imperial_navy_weapons_01", true)
    elseif (screenID == "train_imperial_procedures_s01_tier1") then
        SpaceHelpers:grantSpaceSkill(pPlayer, "pilot_imperial_navy_procedures_01", true)
    elseif (screenID == "train_imperial_droid_s01_tier1") then
        SpaceHelpers:grantSpaceSkill(pPlayer, "pilot_imperial_navy_droid_01", true)
    -- Duty mission starts
    elseif (screenID == "start_imperial_destroy_duty_mission") then
        destroy_duty_naboo_imperial_6:startQuest(pPlayer, pNpc)
    elseif (screenID == "start_imperial_escort_duty_mission") then
        escort_duty_naboo_imperial_7:startQuest(pPlayer, pNpc)
    end

    return clonedConversation
end