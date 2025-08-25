/*
    Copyright <SWGEmu>
    See file COPYING for copying conditions.
*/

#ifndef FIXJEDICOMMAND_H_
#define FIXJEDICOMMAND_H_

#include "server/zone/objects/creature/commands/QueueCommand.h"
#include "system/lang/String.h"

/**
 * Admin-only command:
 *   /fixjedi
 *   /fixjedi self
 *   /fixjedi <FirstName>
 *
 * Sets a “baseline” HAM for Jedi toons.
 */
class FixJediCommand : public QueueCommand {
public:
    FixJediCommand(const String& name, ZoneProcessServer* server)
        : QueueCommand(name, server) {}

    virtual int doQueueCommand(CreatureObject* creature,
                               const uint64& target,
                               const UnicodeString& arguments) const;
};

#endif /* FIXJEDICOMMAND_H_ */
