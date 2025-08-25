/*
    SWGEmu Core3 - FixJediCommand (implementation)
    Usage:
      /fixjedi               -> applies to yourself
      /fixjedi (with target) -> applies to your current target
*/

#include "FixJediCommand.h"
#include "server/zone/ZoneProcessServer.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"

FixJediCommand::FixJediCommand(const String& name, ZoneProcessServer* server)
    : QueueCommand(name, server) {
}

int FixJediCommand::doQueueCommand(CreatureObject* creature,
                                   const uint64& targetID,
                                   const UnicodeString& /*arguments*/) const {

    if (creature == nullptr || !creature->isPlayerCreature())
        return GENERALERROR;

    // Permission: GM+ (>=9)
    PlayerObject* adminGhost = creature->getPlayerObject();
    if (adminGhost == nullptr || adminGhost->getAdminLevel() < 9) {
        creature->sendSystemMessage("You do not have permission to use /fixjedi.");
        return INVALIDPARAMETERS;
    }

    // Resolve target: default self; use selected target if it’s a creature
    CreatureObject* target = creature;
    if (targetID != 0) {
        ZoneServer* zs = server->getZoneServer();
        if (zs != nullptr) {
            SceneObject* so = zs->getObject(targetID);
            if (so != nullptr && so->isCreatureObject())
                target = cast<CreatureObject*>(so);
        }
    }

    if (target == nullptr || !target->isPlayerCreature()) {
        creature->sendSystemMessage("Invalid target.");
        return INVALIDPARAMETERS;
    }

    PlayerObject* ghost = target->getPlayerObject();
    if (ghost == nullptr) {
        creature->sendSystemMessage("Target has no player object.");
        return GENERALERROR;
    }

    if (ghost->getJediState() <= 0) {
        creature->sendSystemMessage(target->getFirstName() + " is not flagged as Jedi.");
        return INVALIDPARAMETERS;
    }

    // Jedi brawler-ish baseline (HAM + secondary)
    const int jediBaseline[9] = { 1100, 900, 650, 600, 600, 500, 500, 450, 450 };
    for (int i = 0; i < 9; ++i) {
        target->setBaseHAM(i, jediBaseline[i], false);
        target->setHAM(i,      jediBaseline[i], false);
        target->setMaxHAM(i,   jediBaseline[i], false);
    }

    target->sendSystemMessage("Your Jedi baseline attributes have been set.");
    if (target != creature)
        creature->sendSystemMessage("Applied Jedi baseline to " + target->getFirstName() + ".");

    return SUCCESS;
}
