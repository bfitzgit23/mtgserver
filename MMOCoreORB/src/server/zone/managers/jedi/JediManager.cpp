/*
                Copyright <SWGEmu>
        See file COPYING for copying conditions.
*/

#include "JediManager.h"
#include "server/zone/managers/director/DirectorManager.h"
#include "server/zone/objects/player/PlayerObject.h"    // for get/setJediState
#include "server/zone/objects/scene/SceneObject.h"      // for useItem()

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
    Reference<LuaFunction*> fn = luaEngine->createFunction(getJediManagerName(), "onPlayerCreated", 0);
    *fn << creature;
    fn->callFunction();
}

void JediManager::onSkillRevoked(CreatureObject* creature, Skill* skill) {
    Lua* luaEngine = DirectorManager::instance()->getLuaInstance();
    Reference<LuaFunction*> fn = luaEngine->createFunction(getJediManagerName(), "onSkillRevoked", 0);
    *fn << creature;
    *fn << skill;
    fn->callFunction();
}

void JediManager::onPlayerLoggedIn(CreatureObject* creature) {
    Lua* luaEngine = DirectorManager::instance()->getLuaInstance();
    Reference<LuaFunction*> fn = luaEngine->createFunction(getJediManagerName(), "onPlayerLoggedIn", 0);
    *fn << creature;
    fn->callFunction();

    // One-time baseline fixer for existing Jedi
    applyBaselineIfNeeded(creature);
}

void JediManager::onPlayerLoggedOut(CreatureObject* creature) {
    Lua* luaEngine = DirectorManager::instance()->getLuaInstance();
    Reference<LuaFunction*> fn = luaEngine->createFunction(getJediManagerName(), "onPlayerLoggedOut", 0);
    *fn << creature;
    fn->callFunction();
}

void JediManager::checkForceStatusCommand(CreatureObject* creature) {
    Lua* luaEngine = DirectorManager::instance()->getLuaInstance();
    Reference<LuaFunction*> fn = luaEngine->createFunction(getJediManagerName(), "checkForceStatusCommand", 0);
    *fn << creature;
    fn->callFunction();
}

// NOTE: unqualified SceneObject* to match the header
void JediManager::useItem(SceneObject* item, const int itemType, CreatureObject* creature) {
    Lua* luaEngine = DirectorManager::instance()->getLuaInstance();
    Reference<LuaFunction*> fn = luaEngine->createFunction(getJediManagerName(), "useItem", 0);
    *fn << item;
    *fn << itemType;
    *fn << creature;
    fn->callFunction();
}

bool JediManager::canLearnSkill(CreatureObject* creature, const String& skillName) {
    Lua* luaEngine = DirectorManager::instance()->getLuaInstance();
    Reference<LuaFunction*> fn = luaEngine->createFunction(getJediManagerName(), "canLearnSkill", 1);
    *fn << creature;
    *fn << skillName;

    lua_State* L = fn->callFunction();
    bool result = lua_toboolean(L, -1);
    lua_pop(L, 1);
    return result;
}

bool JediManager::canSurrenderSkill(CreatureObject* creature, const String& skillName) {
    Lua* luaEngine = DirectorManager::instance()->getLuaInstance();
    Reference<LuaFunction*> fn = luaEngine->createFunction(getJediManagerName(), "canSurrenderSkill", 1);
    *fn << creature;
    *fn << skillName;

    lua_State* L = fn->callFunction();
    bool result = lua_toboolean(L, -1);
    lua_pop(L, 1);
    return result;
}

void JediManager::onFSTreeCompleted(CreatureObject* creature, const String& branch) {
    Lua* luaEngine = DirectorManager::instance()->getLuaInstance();
    Reference<LuaFunction*> fn = luaEngine->createFunction(getJediManagerName(), "onFSTreeCompleted", 0);
    *fn << creature;
    *fn << branch;
    fn->callFunction();
}

/**
 * Login-time fixer to raise existing Jedi to your baseline if needed.
 * Idempotent using stored-int key "jedi.baselineFix2".
 */
void JediManager::applyBaselineIfNeeded(CreatureObject* creature) {
    if (creature == nullptr || !creature->isPlayerCreature())
        return;

    ManagedReference<PlayerObject*> ghost = creature->getPlayerObject();
    if (ghost == nullptr)
        return;

    // Skip if already applied in the past (new key)
    if (creature->getStoredInt("jedi.baselineFix2") == 1)
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
            creature->hasSkill("force_discipline_lightsaber_novice"); // alt naming on some forks
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

    // WRITE LOCK while mutating creature stats (single-arg Locker on your fork)
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

    if (changed) {
        // mark one-time application to avoid re-running every login
        creature->setStoredInt("jedi.baselineFix2", 1);
        // Optional player notify:
        // creature->sendSystemMessage("Jedi baseline stats applied.");
    }
}
