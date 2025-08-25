/*
    SWGEmu Core3 - FixJediCommand (header)
    Declares /fixjedi GM command
*/

#pragma once

#include "server/zone/objects/creature/commands/QueueCommand.h"
#include "server/zone/ZoneProcessServer.h"

class FixJediCommand : public QueueCommand {
public:
    // Only declared here, implemented in FixJediCommand.cpp
    FixJediCommand(const String& name, ZoneProcessServer* server);

    int doQueueCommand(CreatureObject* creature,
                       const uint64& targetID,
                       const UnicodeString& arguments) const override;
};
