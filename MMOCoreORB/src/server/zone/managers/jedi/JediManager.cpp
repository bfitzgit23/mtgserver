/*
                Copyright <SWGEmu>
        See file COPYING for copying conditions.
*/

#include "JediManager.h"
#include "server/zone/managers/director/DirectorManager.h"
#include "server/zone/objects/player/PlayerObject.h" // needed for setJediState()
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
 * One-time via objVar: "jedi.baselineFix2" (new key so it runs after this update).
 */
void JediManager::applyBaselineIfNeeded(CreatureObject* creature) {
    if (creature == nullptr || !creature->isPlayerCreature())
        return;

    ManagedReference<PlayerObject*> ghost = creature->getPlayerObject();
    if (ghost == nullptr)
        return;

    // Skip if already applied in the past (new key)
    if (creature->hasObjVar("jedi.baselineFix2"))
        return;

    // Detect Jedi robustly: state or fallback to skills
    bool isJedi = false;
    try {
        isJedi = (ghost->getJediState() > 0);
    } catch (...) {
        // getJediState may not exist on some forks
    }

    if (!isJedi) {
        isJedi =
            creature->hasSkill("force_title_jedi_novice") ||
            creature->hasSkill("jedi_padawan") ||
            creature->hasSkill("force_sensitive_novice") ||
            creature->hasSkill("force_discipline_light_saber_novice") ||
            creature->hasSkill("force_discipline_lightsaber_novice"); // alt spelling on some forks
    }

    if (!isJedi)
        return;

    // Ensure state is set if your fork uses it
    try {
        if (ghost->getJediState() < 4)
            ghost->setJediState(4);
    } catch (...) {
        // ignore if not present on your fork
    }

    // Target baseline (pool order 0..8 in Core3 HAM arrays)
    static const int target[9] = { 1100, 900, 650, 600, 600, 500, 500, 450, 450 };

    // WRITE LOCK while mutating creature stats (thread-safe)
    Locker _writeLock(creature, true);

    bool changed = false;
    for (int i = 0; i < 9; ++i) {
        int curMax = creature->getMaxHAM(i);
        if (curMax < target[i]) {
            creature->setBaseHAM(i, target[i], false);
            creature->setHAM(i,      target[i], false);
            creature->setMaxHAM(i,   target[i], false);
            changed = true;
        }
    }

    if (changed) {
        // mark one-time application to avoid re-running every login
        creature->setObjVar("jedi.baselineFix2", 1);
        // Optional: notify the player
        // creature->sendSystemMessage("Jedi baseline stats applied.");
    }
}
