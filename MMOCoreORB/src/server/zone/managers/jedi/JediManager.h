/*
                Copyright <SWGEmu>
        See file COPYING for copying conditions.*/

#ifndef JEDIMANAGER_H_
#define JEDIMANAGER_H_

#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/scene/SceneObject.h"   // for SceneObject* in useItem()
#include "engine/lua/Lua.h"                           // use the real Lua type

// Minimal forward declaration used by function signatures
class Skill;

namespace server {
namespace zone {
namespace managers {
namespace jedi {

/**
 * Singleton class with a common interface for all different Jedi progression systems.
 */
class JediManager : public Singleton<JediManager>, public Logger, public Object, public ReadWriteLock {
private:
	// The Jedi progression type currently configured.
	AtomicInteger jediProgressionType;

	// The name of the jedi manager class in Lua.
	String jediManagerName;

	AtomicBoolean loaded{false};

	// Setup Lua global values.
	static void setupLuaValues(Lua* luaEngine);

public:
	enum {
		NOJEDIPROGRESSION,        // Jedi progression not available.
		HOLOGRINDJEDIPROGRESSION, // Master five random professions.
		VILLAGEJEDIPROGRESSION,   // Village system.
		CUSTOMJEDIPROGRESSION     // Custom defined jedi progression system.
	};

	enum {
		ITEMHOLOCRON,         // Item type holocron.
		ITEMWAYPOINTDATAPAD,  // Item type waypoint datapad.
		ITEMTHEATERDATAPAD    // Item type theater datapad.
	};

	JediManager();
	~JediManager();

	// Load the Jedi Manager configuration.
	void loadConfiguration(Lua* luaEngine);

	// Lifecycle hooks.
	void onPlayerCreated(CreatureObject* creature);
	void onPlayerLoggedIn(CreatureObject* creature);
	void onPlayerLoggedOut(CreatureObject* creature);
	void onSkillRevoked(CreatureObject* creature, Skill* skill);

	// Commands.
	void checkForceStatusCommand(CreatureObject* creature);

	// Accessors.
	const String& getJediManagerName();
	int getJediProgressionType();
	void setJediManagerName(const String& name);

	// Items related to jedi progression.
	void useItem(SceneObject* item, const int itemType, CreatureObject* creature);

	// Skill gates.
	bool canLearnSkill(CreatureObject* creature, const String& skillName);
	bool canSurrenderSkill(CreatureObject* creature, const String& skillName);

	// FS tree completion.
	void onFSTreeCompleted(CreatureObject* creature, const String& branch);

	// Login-time fixer to raise existing Jedi to the configured baseline if needed.
	void applyBaselineIfNeeded(CreatureObject* creature);
};

}  // namespace jedi
}  // namespace managers
}  // namespace zone
}  // namespace server

using namespace server::zone::managers::jedi;

#endif /* JEDIMANAGER_H_ */
