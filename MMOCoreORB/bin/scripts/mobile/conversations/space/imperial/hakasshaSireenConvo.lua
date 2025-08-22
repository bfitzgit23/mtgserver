hakassha_sireen_convo_template = ConvoTemplate:new {
    initialScreen = "", -- Determined by the handler
    templateType = "Lua",
    luaClassHandler = "hakasshaSireenConvoHandler", -- This will be the new Lua handler file
    screens = {}
}

-- Initial screens for Hakassha Sireen
hakassha_sireen_greet_imperial = ConvoScreen:new {
    id = "greet_imperial",
    leftDialog = "Welcome, pilot, to the Black Epsilon Squadron! Commander Sireen at your service. Are you ready to begin your advanced training, or do you have other matters to discuss?",
    stopConversation = "false",
    options = {
        {"I am ready for my advanced training.", "offer_tier1_training"},
        {"What kind of missions do you have?", "offer_duty_missions"},
        {"I have other matters.", "goodbye"}
    }
}
hakassha_sireen_convo_template:addScreen(hakassha_sireen_greet_imperial);

hakassha_sireen_offer_tier1_training = ConvoScreen:new {
    id = "offer_tier1_training",
    leftDialog = "Very well. What aspect of Imperial piloting do you wish to master?",
    stopConversation = "false",
    options = {
        {"I am interested in Imperial Starships basics.", "train_imperial_fighters_s01_tier1"},
        {"I am interested in Imperial Weapons basics.", "train_imperial_weapons_s01_tier1"},
        {"I am interested in Imperial Procedures basics.", "train_imperial_procedures_s01_tier1"},
        {"I am interested in Imperial Droid basics.", "train_imperial_droid_s01_tier1"},
        {"That's enough for now.", "goodbye"}
    }
}
hakassha_sireen_convo_template:addScreen(hakassha_sireen_offer_tier1_training);

-- Screens for each skill branch (these might transition back to offer_tier1_training)
hakassha_sireen_train_imperial_fighters_s01_tier1 = ConvoScreen:new {
    id = "train_imperial_fighters_s01_tier1",
    leftDialog = "Excellent choice. Training granted.",
    stopConversation = "false",
    options = {
        {"What else can I learn?", "offer_tier1_training"},
        {"Thank you, Commander.", "goodbye"}
    }
}
hakassha_sireen_convo_template:addScreen(hakassha_sireen_train_imperial_fighters_s01_tier1);

hakassha_sireen_train_imperial_weapons_s01_tier1 = ConvoScreen:new {
    id = "train_imperial_weapons_s01_tier1",
    leftDialog = "Affirmative. Weapons training granted.",
    stopConversation = "false",
    options = {
        {"What else can I learn?", "offer_tier1_training"},
        {"Thank you, Commander.", "goodbye"}
    }
}
hakassha_sireen_convo_template:addScreen(hakassha_sireen_train_imperial_weapons_s01_tier1);

hakassha_sireen_train_imperial_procedures_s01_tier1 = ConvoScreen:new {
    id = "train_imperial_procedures_s01_tier1",
    leftDialog = "Understood. Procedures training granted.",
    stopConversation = "false",
    options = {
        {"What else can I learn?", "offer_tier1_training"},
        {"Thank you, Commander.", "goodbye"}
    }
}
hakassha_sireen_convo_template:addScreen(hakassha_sireen_train_imperial_procedures_s01_tier1);

hakassha_sireen_train_imperial_droid_s01_tier1 = ConvoScreen:new {
    id = "train_imperial_droid_s01_tier1",
    leftDialog = "Droid interface training granted. Proceed with caution.",
    stopConversation = "false",
    options = {
        {"What else can I learn?", "offer_tier1_training"},
        {"Thank you, Commander.", "goodbye"}
    }
}
hakassha_sireen_convo_template:addScreen(hakassha_sireen_train_imperial_droid_s01_tier1);

hakassha_sireen_offer_duty_missions = ConvoScreen:new {
    id = "offer_duty_missions",
    leftDialog = "We always have patrols and escorts that need attention. Which type of mission are you looking for?",
    stopConversation = "false",
    options = {
        {"I'll take a destruction duty mission.", "start_imperial_destroy_duty_mission"}, -- Renamed for clarity
        {"I'll take an escort duty mission.", "start_imperial_escort_duty_mission"}, -- Renamed for clarity
        {"Never mind.", "goodbye"}
    }
}
hakassha_sireen_convo_template:addScreen(hakassha_sireen_offer_duty_missions);

hakassha_sireen_start_imperial_destroy_duty_mission = ConvoScreen:new {
    id = "start_imperial_destroy_duty_mission",
    leftDialog = "A destruction duty mission it is. Return when complete.",
    stopConversation = "true",
    options = {}
}
hakassha_sireen_convo_template:addScreen(hakassha_sireen_start_imperial_destroy_duty_mission);

hakassha_sireen_start_imperial_escort_duty_mission = ConvoScreen:new {
    id = "start_imperial_escort_duty_mission",
    leftDialog = "An escort duty mission it is. Good luck, pilot.",
    stopConversation = "true",
    options = {}
}
hakassha_sireen_convo_template:addScreen(hakassha_sireen_start_imperial_escort_duty_mission);


-- Complete skill box screen (for when player has learned all Tier 1 skills)
hakassha_sireen_completed_all_tier1 = ConvoScreen:new {
    id = "completed_all_tier1",
    leftDialog = "Excellent work, pilot! You have mastered all the basic Imperial piloting skills. Your next assignment and advanced training will come from a higher-ranking officer.",
    stopConversation = "true",
    options = {
        {"Thank you, Commander.", "goodbye"}
    }
}
hakassha_sireen_convo_template:addScreen(hakassha_sireen_completed_all_tier1);

-- Screen if player is not an Imperial or Black Epsilon (fallback)
hakassha_sireen_no_business = ConvoScreen:new {
    id = "no_business",
    leftDialog = "I'm afraid I have no business with you, civilian. Only authorized Imperial pilots of Black Epsilon Squadron are to approach this post.",
    stopConversation = "true",
    options = {}
}
hakassha_sireen_convo_template:addScreen(hakassha_sireen_no_business);

hakassha_sireen_goodbye = ConvoScreen:new {
    id = "goodbye",
    leftDialog = "Dismissed, pilot. The Empire is grateful for your service.",
    stopConversation = "true",
    options = {}
}
hakassha_sireen_convo_template:addScreen(hakassha_sireen_goodbye);

addConversationTemplate("hakassha_sireen_convo_template", hakassha_sireen_convo_template);