/*
    Copyright <SWGEmu>
    See file COPYING for copying conditions.
*/
#include "server/zone/managers/jedi/JediManager.h"

#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/managers/skill/SkillManager.h"

using namespace server::zone::managers::jedi;  // explicit, safer across forks

// --- Helpers ---
static inline bool isJediCharacter(PlayerObject* ghost) {
    if (!ghost) return false;
    // Adjust tokens to your repo if different
    return ghost->hasSkill("force_sensitive_novice")
        || ghost->hasSkill("jedi_padawan")
        || ghost->hasSkill("force_title_jedi_novice");
}

// Tanky baseline: Health, Action, Mind, Str, Con, Qui, Sta, Int, Pre
static const int JEDI_TANKY_BASELINE[9] = { 1000, 850, 850, 65, 65, 55, 65, 55, 55 };

// --- Class impl ---
JediManager::JediManager() : Logger("JediManager") {
    setLogging(false);
    setGlobalLogging(false);
    jediProgressionType.set(NOJEDIPROGRESSION);
    jediManagerName = "JediManagerDefault";
    loaded.set(false);
}

JediManager::~JediManager() {}

void JediManager::applyBaselineIfNeeded(CreatureObject* creature) {
    if (creature == nullptr || !creature->isPlayerCreature())
        return;

    PlayerObject* ghost = creature->getPlayerObject();
    if (!isJediCharacter(ghost))
        return;

    bool changed = false;

    // 0..8 = Health, Action, Mind, Strength, Constitution, Quickness, Stamina, Intelligence, Presence
    for (int i = 0; i < 9; ++i) {
        int curBase = creature->getBaseHAM(i);
        int target  = JEDI_TANKY_BASELINE[i];

        if (curBase < target) {
            creature->setBaseHAM(i, target, /*sendUpdate*/false);
            creature->setMaxHAM(i,  target, /*sendUpdate*/false);
            creature->setHAM(i,     target, /*sendUpdate*/false);
            changed = true;
        }
    }

    if (changed) {
        try { creature->refreshDerivedStats(); } catch (...) {}
        try { creature->recalculateLevel();     } catch (...) {}
        creature->broadcastPvpStatusBitmask();
        info(true) << "Applied tanky baseline to Jedi " << creature->getDisplayedName();
    }
}

// ---- Minimal, compile-safe stubs below ----
// You can wire real logic later; these satisfy existing call sites.
void JediManager::loadConfiguration(Lua* /*luaEngine*/) {
    // If you have Lua configuration, set it here.
    loaded.set(true);
}

void JediManager::onPlayerCreated(CreatureObject* /*creature*/) {}
void JediManager::onPlayerLoggedIn(CreatureObject* /*creature*/) {}
void JediManager::onPlayerLoggedOut(CreatureObject* /*creature*/) {}
void JediManager::onSkillRevoked(CreatureObject* /*creature*/, Skill* /*skill*/) {}
void JediManager::checkForceStatusCommand(CreatureObject* /*creature*/) {}

const String& JediManager::getJediManagerName() { return jediManagerName; }
int JediManager::getJediProgressionType() { return jediProgressionType.get(); }
void JediManager::setJediManagerName(const String& name) { jediManagerName = name; }

void JediManager::useItem(SceneObject* /*item*/, const int /*itemType*/, CreatureObject* /*creature*/) {}

bool JediManager::canLearnSkill(CreatureObject* /*creature*/, const String* /*skillName*/) { return true; }
bool JediManager::canSurrenderSkill(CreatureObject* /*creature*/, const String* /*skillName*/) { return true; }

void JediManager::onFSTreeCompleted(CreatureObject* /*creature*/, const String& /*branch*/) {}
