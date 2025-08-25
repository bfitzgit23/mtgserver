/*
    Copyright <SWGEmu>
    See file COPYING for copying conditions.
*/

#include "FixJediCommand.h"

#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/ZoneServer.h"
#include "server/chat/ChatManager.h"

int FixJediCommand::doQueueCommand(CreatureObject* creature,
                                   const uint64& /*target*/,
                                   const UnicodeString& arguments) const {
    if (!checkStateMask(creature))
        return INVALIDSTATE;

    if (!checkInvalidLocomotions(creature))
        return INVALIDLOCOMOTION;

    if (creature == nullptr || !creature->isPlayerCreature())
        return GENERALERROR;

    // Admin check
    ManagedReference<PlayerObject*> adminGhost = creature->getPlayerObject();
    if (adminGhost == nullptr || adminGhost->getAdminLevel() < 9) {
        creature->sendSystemMessage("You must be an admin to use this command.");
        return GENERALERROR;
    }

    // Parse argument (optional first name or "self")
    String arg = arguments.toString().trim();

    // Default target is self
    ManagedReference<CreatureObject*> targetCreature = creature;

    // If a name was provided (and not 'self'), try to resolve it via ChatManager
    if (!arg.isEmpty() && !arg.equalsIgnoreCase("self")) {
        ZoneServer* zserv = creature->getZoneServer();
        if (zserv != nullptr) {
            ChatManager* chat = zserv->getChatManager();
            if (chat != nullptr) {
                ManagedReference<CreatureObject*> found = chat->getPlayer(arg);
                if (found != nullptr)
                    targetCreature = found;
            }
        }
    }

    if (targetCreature == nullptr) {
        creature->sendSystemMessage("Target player not found.");
        return GENERALERROR;
    }

    // Apply “Jedi Brawler” baseline HAMs
    // Order: 0..8 = Health, Action, Mind, Strength, Constitution, Quickness, Stamina, Intelligence, Presence
    const int jediBrawler[9] = { 1100, 900, 650, 600, 600, 500, 500, 450, 450 };

    for (int i = 0; i < 9; ++i) {
        targetCreature->setBaseHAM(i, jediBrawler[i], false);
        targetCreature->setHAM(i,      jediBrawler[i], false);
        targetCreature->setMaxHAM(i,   jediBrawler[i], false);
    }

    // Let the client know
    creature->sendSystemMessage("Jedi HAM baseline fixed for " + targetCreature->getFirstName() + ".");

    return SUCCESS;
}
