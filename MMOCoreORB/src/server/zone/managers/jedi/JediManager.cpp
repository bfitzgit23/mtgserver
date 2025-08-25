/*
                Copyright <SWGEmu>
        See file COPYING for copying conditions.
*/

#include "JediManager.h"
#include "server/zone/managers/director/DirectorManager.h"
#include "server/zone/objects/player/PlayerObject.h" // needed for get/setJediState()
#include "server/zone/objects/scene/SceneObject.h"

JediManager::JediManager() : Logger("JediManager") {
    jediProgressionType = NOJEDIPROGRESSION;
    setJediManagerName("JediManager");
}

JediManager::~JediManager() {

}

const String& JediManager::getJediManagerName() {
    ReadLocker locker(this);
    return jediManagerName;
}

int JediManager::getJediProgressionType() {
    ReadLocker locker(this);
    return jediProgressionType;
}

void JediManager::setJediManagerName(const String& name) {
    Locker writeLock(this);
    jediManagerName = name;
}

void JediManager::setupLuaValues(Lua* luaEngine) {
    luaEngine->setGlobalInt("NOJEDIPROGRESSION", JediManager::NOJEDIPROGRESSION);
    luaEngine->setGlobalInt("HOLOGRINDJEDIPROGRESSION", JediManager::HOLOGRINDJEDIPROGRESSION);
    luaEngine->setGlobalInt("VILLAGEJEDIPROGRESSION", JediManager::VILLAGEJEDIPROGRESSION);
    luaEngine->setGlobalInt("CUSTOMJEDIPROGRESSION", JediManager::CUSTOMJEDIPROGRESSION);
    luaEngine->setGlobalInt("ITEMHOLOCRON", JediManager::ITEMHOLOCRON);
    luaEngine->setGlobalInt("ITEMWAYPOINTDATAPAD", JediManager::ITEMWAYPOINTDATAPAD);
    luaEngine->setGlobalInt("ITEMTHEATERDATAPAD", JediManager::ITEMTHEATERDATAPAD);
}

void JediManager::loadConfiguration(Lua* luaEngine) {
    setupLuaValues(luaEngine);

    luaEngine->runFile("scripts/managers/jedi/jedi_manager.lua");

    jediProgressionType = luaEngine->getGlobalInt(String("jediProgressionType"));

    switch (jediProgressionType) {
    case HOLOGRINDJEDIPROGRESSION:
        luaEngine->runFile("scripts/managers/jedi/hologrind_jedi_manager.lua");
        break;
    case VILLAGEJEDIPROGRESSION:
        luaEngine->runFile("scripts/managers/jedi/village_jedi_manager.lua");
        break;
    case CUSTOMJEDIPROGRESSION:
        luaEngine->runFile(luaEngine->getGlobalString("customJediProgressionFile"));
        break;
    default:
        break;
    }

    if (loaded.compareAndSet(false, true)) {
        auto managerName = luaEngine->getGlobalString(String("jediManagerName"));
        setJediManagerName(managerName);
    }

    info() << Thread::getCurrentThread()->getName() << " loaded.";
}

void JediManager::onPlayerCreated(CreatureObject* creature) {
    Lua* luaEngine = DirectorManager::instance()->getLuaInstance();
    Reference<LuaFunction*> luaOnPlayerCreated = luaEngine->createFunction(getJediManagerName(), "onPlayerCreated", 0);
    *luaOnPlayerCreated << creature;
    luaOnPlayerCreated->callFunction();
}

void JediManager::onSkillRevoked(CreatureObject* creature, Skill* skill) {
    Lua* luaEngine = DirectorManager::instance()->getLuaInstance();
    Reference<LuaFunction*> luaOnSkillRevoked = luaEngine->createFunction(getJediManagerName(), "onSkillRevoked", 0);
    *luaOnSkillRevoked << creature;
    *luaOnSkillRevoked << skill;
    luaOnSkillRevoked->callFunction();
}

void JediManager::onPlayerLoggedIn(CreatureObject* creature) {
    Lua* luaEngine = DirectorManager::instance()->getLuaInstance();
    Reference<LuaFunction*> luaOnPlayerLoggedIn = luaEngine->createFunction(getJediManagerName(), "onPlayerLoggedIn", 0);
    *luaOnPlayerLoggedIn << creature;
    luaOnPlayerLoggedIn->callFunction();

    // Baseline fixer runs at login for existing Jedi
    applyBaselineIfNeeded(creature);
}

void JediManager::onPlayerLoggedOut(CreatureObject* creature) {
    Lua* luaEngine = DirectorManager::instance()->getLuaInstance();
    Reference<LuaFunction*> luaOnPlayerLoggedOut = luaEngine->createFunction(getJediManagerName(), "onPlayerLoggedOut", 0);
    *luaOnPlayerLoggedOut << creature;
    luaOnPlayerLoggedOut->callFunction();
}

void JediManager::checkForceStatusCommand(CreatureObject* creature) {
    Lua* luaEngine = DirectorManager::instance()->getLuaInstance();
    Reference<LuaFunction*> luaCheckForceStatusCommand = luaEngine->createFunction(getJediManagerName(), "checkForceStatusCommand", 0);
    *luaCheckForceStatusCommand << creature;
    luaCheckForceStatusCommand->callFunction();
}

// NOTE: unqualified SceneObject* to match the .h
void JediManager::useItem(SceneObject* item, const int itemType, CreatureObject* creature) {
    Lua* luaEngine = DirectorManager::instance()->getLuaInstance();
    Reference<LuaFunction*> luaUseItem = luaEngine->createFunction(getJediManagerName(), "useItem", 0);
    *luaUseItem << item;
    *luaUseItem << itemType;
    *luaUseItem << creature;
    luaUseItem->callFunction();
}

bool JediManager::canLearnSkill(CreatureObject* creature, const String& skillName) {
    Lua* luaEngine = DirectorManager::instance()->getLuaInstance();
    Reference<LuaFunction*> luaStartTask = luaEngine->createFunction(getJediManagerName(), "canLearnSkill", 1);
    *luaStartTask << creature;
    *luaStartTask << skillName;

    lua_State* L = luaStartTask->callFunction();
    bool result = lua_toboolean(L, -1);
    lua_pop(L, 1);
    return result;
}

bool JediManager::canSurrenderSkill(CreatureObject* creature, const String& skillName) {
    Lua* luaEngine = DirectorManager::instance()->getLuaInstance();
    Reference<LuaFunction*> luaStartTask = luaEngine->createFunction(getJediManagerName(), "canSurrenderSkill", 1);
    *luaStartTask << creature;
    *luaStartTask << skillName;

    lua_State* L = luaStartTask->callFunction();
    bool result = lua_toboolean(L, -1);
    lua_pop(L, 1);
    return result;
}

void JediManager::onFSTreeCompleted(CreatureObject* creature, const String& branch) {
    Lua* luaEngine = DirectorManager::instance()->getLuaInstance();
    Reference<LuaFunction*> luaStartTask = luaEngine->createFunction(getJediManagerName(), "onFSTreeCompleted", 0);
    *luaStartTask << creature;
    *luaStartTask << branch;
    luaStartTask->callFunction();
}

/**
 * Login-time fixer to raise existing Jedi to your baseline if needed.
 * One-time via storedInt guard "jedi.baselineFix2".
 * Also logs decisions and nudges posture to refresh client HAM display.
 */
void JediManager::applyBaselineIfNeeded(CreatureObject* creature) {
    if (creature == nullptr || !creature->isPlayerCreature()) {
        info() << "[JediFix] Skip: null/not player.";
        return;
    }

    String who;
    try { who = creature->getFirstName(); } catch (...) {}
    if (who.isEmpty()) who = String::valueOf(creature->getObjectID());

    ManagedReference<PlayerObject*> ghost = creature->getPlayerObject();
    if (ghost == nullptr) {
        info() << "[JediFix] " << who << ": Skip (no PlayerObject).";
        return;
    }

    // One-time guard (stored int is widely available across forks)
    try {
        if (creature->getStoredInt("jedi.baselineFix2") == 1) {
            info() << "[JediFix] " << who << ": Already applied. Skip.";
            return;
        }
    } catch (...) { /* if storedInt not present, we’ll just proceed idempotently */ }

    // Robust Jedi detection
    bool isJedi = false;
    int jediState = 0;
    try { jediState = ghost->getJediState(); } catch (...) {}
    if (jediState > 0) isJedi = true;

    const char* skillChecks[] = {
        "jedi_padawan",
        "force_sensitive_novice",
        "force_title_jedi_novice",
        "force_title_jedi_master",
        "force_discipline_light_saber_novice",
        "force_discipline_lightsaber_novice",
        "force_ability_melee_defense_01",
        "force_ability_melee_defense_02"
    };

    if (!isJedi) {
        for (auto s : skillChecks) {
            try {
                if (creature->hasSkill(s)) {
                    isJedi = true;
                    break;
                }
            } catch (...) {}
        }
    }

    if (!isJedi) {
        info() << "[JediFix] " << who << ": Not detected as Jedi (state=" << jediState << ").";
        return;
    }

    // Ensure state is permissive (best-effort)
    try {
        if (jediState < 4) {
            ghost->setJediState(4);
            info() << "[JediFix] " << who << ": JediState -> 4";
        }
    } catch (...) {}

    // Target baseline (pool order 0..8 in Core3 HAM arrays)
    static const int target[9] = { 1100, 900, 650, 600, 600, 500, 500, 450, 450 };

    // WRITE LOCK while mutating creature stats
    Locker _lock(creature);

    bool changed = false;
    for (int i = 0; i < 9; ++i) {
        const int curMax = creature->getMaxHAM(i);
        if (curMax < target[i]) {
            creature->setBaseHAM(i, target[i], false);
            creature->setHAM(i,      target[i], false);
            creature->setMaxHAM(i,   target[i], false);
            changed = true;
        }
    }

    if (!changed) {
        info() << "[JediFix] " << who << ": Already >= baseline. No changes.";
        return;
    }

    // Mark one-time application if available
    try { creature->setStoredInt("jedi.baselineFix2", 1); } catch (...) {}

    // Light refresh so client sees updated pools
    try {
        auto posture = creature->getPosture();
        creature->setPosture(posture, true); // often forces client update
    } catch (...) {}

    info() << "[JediFix] " << who << ": Baseline applied (>= "
           << target[0] << "/" << target[1] << "/" << target[2] << ").";
}
