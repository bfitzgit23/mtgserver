/*
    Copyright <SWGEmu>
    See file COPYING for copying conditions.
*/

#ifndef FIXJEDICOMMAND_H_
#define FIXJEDICOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/ZoneProcessServer.h"
// Fixed path:
#include "server/zone/objects/creature/commands/QueueCommand.h"

class FixJediCommand : public QueueCommand {
public:
    FixJediCommand(const String& name, ZoneProcessServer* server)
        : QueueCommand(name, server) {
    }

    int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const override;
};

#endif /* FIXJEDICOMMAND_H_ */
