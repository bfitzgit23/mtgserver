/*
                Copyright <SWGEmu>
        See file COPYING for copying conditions.
*/

#ifndef JEDIMANAGER_H_
#define JEDIMANAGER_H_

#include "server/zone/objects/creature/CreatureObject.h"

// Forward declarations kept minimal and correctly scoped.
class Lua;
class Skill;

namespace server {
namespace zone {
namespace objects {
namespace scene {
class SceneObject; // correct namespace for SceneObject
}
}
}
}

namespace server {
namespace zone {
namespace managers {
namespace jedi {

/**
 * Singleton class with a common interface for all different Jedi progression systems.
 */
class JediManager : public Singleton<JediManager>, public Logger, public Object, public ReadWriteLock {
private:
    /**
     * The Jedi progression type currently configured.
     */
    AtomicInteger jediProgressionType;

    /**
     * The name of the jedi manager class in Lua.
     */
    String jediManagerName;

    AtomicBoolean loaded{false};

    /**
     * Setup Lua global values.
     * @param luaEngine the lua instance.
     */
    static void setupLuaValues(Lua* luaEngine);

public:
    enum {
        NOJEDIPROGRESSION,         // Jedi progression not available.
        HOLOGRINDJEDIPROGRESSION,  // Master five random professions.
        VILLAGEJEDIPROGRESSION,    // Village system.
        CUSTOMJEDIPROGRESSION      // Custom progression.
    };

    enum {
        ITEMHOLOCRON,
        ITEMWAYPOINTDATAPAD,
        ITEMTHEATERDATAPAD
    };

    JediManager();
    ~JediManager();

    void loadConfiguration(Lua* luaEngine);

    void onPlayerCreated(CreatureObject* creature);
    void onPlayerLoggedIn(CreatureObject* creature);
    void onPlayerLoggedOut(CreatureObject* creature);
    void onSkillRevoked(CreatureObject* creature, Skill* skill);

    void checkForceStatusCommand(CreatureObject* creature);

    const String& getJediManagerName();
    int getJediProgressionType();
    void setJediManagerName(const String& name);

    void useItem(server::zone::objects::scene::SceneObject* item, const int itemType, CreatureObject* creature);

    bool canLearnSkill(CreatureObject* creature, const String& skillName);
    bool canSurrenderSkill(CreatureObject* creature, const String& skillName);

    void onFSTreeCompleted(CreatureObject* creature, const String& branch);

    /**
     * Fix baseline stats for Jedi (used at login).
     * Implemented in JediManager.cpp; called by onPlayerLoggedIn.
     */
    void applyBaselineIfNeeded(CreatureObject* creature);
};

}  // namespace jedi
}  // namespace managers
}  // namespace zone
}  // namespace server

#endif /* JEDIMANAGER_H_ */
