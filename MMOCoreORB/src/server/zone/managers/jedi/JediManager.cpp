/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#include "JediManager.h"
#include "server/zone/managers/director/DirectorManager.h"

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

	// Ensure existing Jedi get the baseline HAM if they predate the change.
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

// --- Added: login-time fixer so existing Jedi get the new baseline on login ---
void JediManager::applyBaselineIfNeeded(CreatureObject* creature) {
	if (creature == nullptr || !creature->isPlayerCreature())
		return;

	// Heuristic: treat as Jedi if they have any core force skills or are flagged as Jedi
	bool isJedi = false;

	// Cheap checks that don't require string allocations if possible
	if (creature->hasSkill("force_sensitive_novice") ||
	    creature->hasSkill("force_discipline_light_saber_novice") ||
	    creature->hasSkill("force_title_jedi_novice") ||
	    creature->hasSkill("force_title_jedi_rank_01") ||
	    creature->hasSkill("force_title_jedi_rank_02") ||
	    creature->hasSkill("force_title_jedi_rank_03") ||
	    creature->hasSkill("force_title_jedi_master")) {
		isJedi = true;
	}

	if (!isJedi) {
		// nothing to do
		return;
	}

	// Target baseline we want for Jedi (brawler-leaning)
	// Order: 0..8 = Health, Action, Mind, Strength, Constitution, Quickness, Stamina, Intelligence, Presence
	const int target[9] = { 1100, 900, 650, 600, 600, 500, 500, 450, 450 };

	bool changed = false;

	for (int i = 0; i < 9; ++i) {
		const int curBase = creature->getBaseHAM(i);
		const int curMax  = creature->getMaxHAM(i);

		// Only raise up if below our new floor; never reduce stats here.
		if (curBase < target[i]) {
			creature->setBaseHAM(i, target[i], false);
			changed = true;
		}
		if (curMax < target[i]) {
			creature->setMaxHAM(i, target[i], false);
			changed = true;
		}
		// Clamp current HAM up to at least the new floor so players don't log in with red bars.
		if (creature->getHAM(i) < target[i]) {
			creature->setHAM(i, target[i], false);
			changed = true;
		}
	}

	if (changed) {
		info() << "applyBaselineIfNeeded: raised Jedi baseline for OID=" << creature->getObjectID();
	}
