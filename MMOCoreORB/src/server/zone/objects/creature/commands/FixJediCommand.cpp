/*
    SWGEmu Core3 - FixJediCommand
    Sets Jedi baseline HAM + secondary attributes for a target or self.

    Usage:
      /fixjedi
      /fixjedi <FirstName>

    Permission: Admin level >= 9
*/

#include "server/zone/objects/creature/commands/QueueCommand.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/ZoneProcessServer.h"

class FixJediCommand : public QueueCommand {
public:
    FixJediCommand(const String& name, ZoneProcessServer* server)
        : QueueCommand(name, server) {
    }

    int doQueueCommand(CreatureObject* creature, const uint64& targetID, const UnicodeString& arguments) const {
        if (creature == nullptr || !creature->isPlayerCreature())
            return GENERALERROR;

        // Permission check: admin level >= 9 (GM)
        PlayerObject* adminGhost = creature->getPlayerObject();
        if (adminGhost == nullptr || adminGhost->getAdminLevel() < 9) {
            creature->sendSystemMessage("You do not have permission to use /fixjedi.");
            return INVALIDPARAMETERS;
        }

        // Resolve target: default self, or provided first name
        String arg = arguments.toString().trim();
        CreatureObject* target = creature;

        if (!arg.isEmpty() && !arg.equalsIgnoreCase("self")) {
            ZoneServer* zoneServer = server->getZoneServer();
            if (zoneServer == nullptr) {
                creature->sendSystemMessage("Server error (zoneserver is null).");
                return GENERALERROR;
            }

            PlayerManager* pm = zoneServer->getPlayerManager();
            if (pm == nullptr) {
                creature->sendSystemMessage("Server error (playermanager is null).");
                return GENERALERROR;
            }

            ManagedReference<CreatureObject*> found = pm->getPlayerByFirstName(arg);
            if (found == nullptr) {
                creature->sendSystemMessage("Target not found or not online: " + arg);
                return INVALIDPARAMETERS;
            }

            target = found;
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

        // Jedi status check: require JediState > 0
        if (ghost->getJediState() <= 0) {
            creature->sendSystemMessage(target->getFirstName() + " is not flagged as Jedi.");
            return INVALIDPARAMETERS;
        }

        // Jedi "brawler-ish" baseline
        const int jediBaseline[9] = { 1100, 900, 650, 600, 600, 500, 500, 450, 450 };

        for (int i = 0; i < 9; ++i) {
            target->setBaseHAM(i, jediBaseline[i], false);
            target->setHAM(i,      jediBaseline[i], false);
            target->setMaxHAM(i,   jediBaseline[i], false);
        }

        String who = target->getFirstName();
        target->sendSystemMessage("Your Jedi baseline attributes have been set.");
        if (target != creature)
            creature->sendSystemMessage("Applied Jedi baseline to " + who + ".");

        return SUCCESS;
    }
};

// Factory (auto-registered by commands.lua)
static QueueCommand* FixJediCommandFactory(const String& name, ZoneProcessServer* server) {
    return new FixJediCommand(name, server);
}
