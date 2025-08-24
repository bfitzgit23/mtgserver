/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#include "JediManager.h"
#include "server/zone/managers/director/DirectorManager.h"
#include "server/zone/managers/skill/SkillManager.h" // [PATCH] needed for skill checks

// --- [PATCH] Helpers to detect Jedi + target baseline ---
// We check via SkillManager so it matches server truth.
static inline bool isJediCharacter(CreatureObject* creature) {
	if (creature == nullptr) return false;
	auto* sm = SkillManager::instance();
	// Adjust tokens if your repo differs
	return sm->hasSkill(creature, "force_sensitive_novice")
	    || sm->hasSkill(creature, "jedi_padawan")
	    || sm->hasSkill(creature, "force_title_jedi_novice");
}

// Order: Health, Action, Mind, Str, Con, Qui, Sta, Int, Pre
static const int JEDI_TANKY_BASELINE[9] = { 1000, 850, 850, 65, 65, 55, 65, 55, 55 };
// --- [/PATCH] ---

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

	info() << Thread::getCurrentThread()->getName() <<  " loaded.";
}

void JediManager::onPlayerCreated(CreatureObject* creature) {
	Lua* lua = DirectorManager::instance()->getLuaInstance();
	Reference<LuaFunction*> luaOnPlayerCreated = lua->createFunction(getJediManagerName(), "onPlayerCreated", 0);
	*luaOnPlayerCreated << creature;

	luaOnPlayerCreated->callFunction();

	// [PATCH] Ensure fresh toons that are Jedi get the baseline right away
	applyBaselineIfNeeded(creature);
}

void JediManager::onSkillRevoked(CreatureObject* creature, Skill* skill) {
	Lua* lua = DirectorManager::instance()->getLuaInstance();
	Reference<LuaFunction*> luaOnSkillRevoked = lua->createFunction(getJediManagerName(), "onSkillRevoked", 0);
	*luaOnSkillRevoked << creature;
	*luaOnSkillRevoked << skill;

	luaOnSkillRevoked->callFunction();
}

void JediManager::onPlayerLoggedIn(CreatureObject* creature) {
	Lua* lua = DirectorManager::instance()->getLuaInstance();
	Reference<LuaFunction*> luaOnPlayerLoggedIn = lua->createFunction(getJediManagerName(), "onPlayerLoggedIn", 0);
	*luaOnPlayerLoggedIn << creature;

	luaOnPlayerLoggedIn->callFunction();

	// [PATCH] Fix up existing characters that are Jedi (raise low HAM to tanky baseline)
	applyBaselineIfNeeded(creature);
}

void JediManager::onPlayerLoggedOut(CreatureObject* creature) {
	Lua* lua = DirectorManager::instance()->getLuaInstance();
	Reference<LuaFunction*> luaOnPlayerLoggedOut = lua->createFunction(getJediManagerName(), "onPlayerLoggedOut", 0);
	*luaOnPlayerLoggedOut << creature;

	luaOnPlayerLoggedOut->callFunction();
}

void JediManager::checkForceStatusCommand(CreatureObject* creature) {
	Lua* lua = DirectorManager::instance()->getLuaInstance();
	Reference<LuaFunction*> luaCheckForceStatusCommand = lua->createFunction(getJediManagerName(), "checkForceStatusCommand", 0);
	*luaCheckForceStatusCommand << creature;

	luaCheckForceStatusCommand->callFunction();
}

void JediManager::useItem(SceneObject* item, const int itemType, CreatureObject* creature) {
	Lua* lua = DirectorManager::instance()->getLuaInstance();
	Reference<LuaFunction*> luaUseItem = lua->createFunction(getJediManagerName(), "useItem", 0);
	*luaUseItem << item;
	*luaUseItem << itemType;
	*luaUseItem << creature;

	luaUseItem->callFunction();
}

bool JediManager::canLearnSkill(CreatureObject* creature, const String& skillName) {
	Lua* lua = DirectorManager::instance()->getLuaInstance();
	Reference<LuaFunction*> luaStartTask = lua->createFunction(getJediManagerName(), "canLearnSkill", 1);
	*luaStartTask << creature;
	*luaStartTask << skillName;

	lua_State* L = luaStartTask->callFunction();

	bool result = lua_toboolean(L, -1);

	lua_pop(L, 1);

	return result;
}

bool JediManager::canSurrenderSkill(CreatureObject* creature, const String& skillName) {
	Lua* lua = DirectorManager::instance()->getLuaInstance();
	Reference<LuaFunction*> luaStartTask = lua->createFunction(getJediManagerName(), "canSurrenderSkill", 1);
	*luaStartTask << creature;
	*luaStartTask << skillName;

	lua_State* L = luaStartTask->callFunction();

	bool result = lua_toboolean(L, -1);

	lua_pop(L, 1);

	return result;
}

void JediManager::onFSTreeCompleted(CreatureObject* creature, const String& branch) {
	Lua* lua = DirectorManager::instance()->getLuaInstance();
	Reference<LuaFunction*> luaStartTask = lua->createFunction(getJediManagerName(), "onFSTreeCompleted", 0);
	*luaStartTask << creature;
	*luaStartTask << branch;

	luaStartTask->callFunction();
}

// =======================
// [PATCH] Implementation
// =======================
void JediManager::applyBaselineIfNeeded(CreatureObject* creature) {
	if (creature == nullptr || !creature->isPlayerCreature())
		return;

	if (!isJediCharacter(creature))
		return;

	bool changed = false;

	// 0..8 = Health, Action, Mind, Strength, Constitution, Quickness, Stamina, Intelligence, Presence
	for (int i = 0; i < 9; ++i) {
		const int target = JEDI_TANKY_BASELINE[i];
		const int cur    = creature->getBaseHAM(i);
		if (cur < target) {
			creature->setBaseHAM(i, target, /*sendUpdate*/false);
			creature->setMaxHAM(i,  target, /*sendUpdate*/false);
			creature->setHAM(i,     target, /*sendUpdate*/false);
			changed = true;
		}
	}

	if (changed) {
		// Keep it portable across forks — avoid calling non-universal helpers
		creature->broadcastPvpStatusBitmask();
		info(true) << "Applied tanky baseline to Jedi " << creature->getDisplayedName();
	}
}
