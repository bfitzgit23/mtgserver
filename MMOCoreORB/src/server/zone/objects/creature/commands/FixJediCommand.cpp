Ben (stormos Dev)
bennji1991
Online

Ben (stormos Dev)
[GEN2]
 — Yesterday at 10:33 PM
no let me try the  3 seperate commands
Zubzy — Yesterday at 10:33 PM
ok
keep me posted
im here to help:)
Ben (stormos Dev)
[GEN2]
 — Yesterday at 10:40 PM
Image
Got this
Zubzy — Yesterday at 10:41 PM
ok checking now!
Ben (stormos Dev)
[GEN2]
 — Yesterday at 10:41 PM
ok
Zubzy — Yesterday at 10:44 PM
SOURCE /full/path/to/MariaDB_Jedi_Skill_Patch.sql;
Ben (stormos Dev)
[GEN2]
 — Yesterday at 10:44 PM
i havent that .sql in the sql folder
or anywhere
Zubzy — Yesterday at 10:45 PM
ok im trying to figure out what sql file it is that has the stats for jedi
Ben (stormos Dev)
[GEN2]
 — Yesterday at 10:45 PM
ya
Zubzy — Yesterday at 10:45 PM
DESCRIBE characters;
try that show me the results
Ben (stormos Dev)
[GEN2]
 — Yesterday at 10:46 PM
Image
Zubzy — Yesterday at 10:47 PM
do u want just toons athat started as jedi r all toons that jedi abilities
Ben (stormos Dev)
[GEN2]
 — Yesterday at 10:49 PM
ya f-ck it
not worth messing with the db imho
Zubzy — Yesterday at 10:50 PM
ok wanna just do the admin commands
Ben (stormos Dev)
[GEN2]
 — Yesterday at 10:50 PM
ya
Zubzy — Yesterday at 10:50 PM
ok ill make announcemnt
u booting ups erver?
Ben (stormos Dev)
[GEN2]
 — Yesterday at 10:51 PM
ya its loading
its up
Zubzy — Yesterday at 11:03 PM
whats the telport command ? try to bring my toon over to eisley
Ben (stormos Dev)
[GEN2]
 — Yesterday at 11:03 PM
/teleporttarget
Zubzy — Yesterday at 11:05 PM
mahe have toi get u to set my toon to admin ... try other toon thats godmode but wouldnt let me tele
Ben (stormos Dev)
[GEN2]
 — Yesterday at 11:05 PM
im not sure we can do that
Zubzy — Yesterday at 11:06 PM
kk ill see whats going on with it
give me a sec
Ben (stormos Dev)
[GEN2]
 — Yesterday at 11:06 PM
ok
Zubzy — Yesterday at 11:08 PM
yea its account side need admin 15 ... not sure why its being glitch wont let me setgod mod .. other toonn i cant teleport but he has god mode ..its weird ,,, hes on msuty lol
Ben (stormos Dev)
[GEN2]
 — Yesterday at 11:09 PM
ok hold on
Zubzy — Yesterday at 11:09 PM
yep
Ben (stormos Dev)
[GEN2]
 — Yesterday at 11:24 PM
ok see if you're admin now
it may not take effect on your current toon idk
Zubzy — Yesterday at 11:24 PM
ok let me relog
Zubzy — Yesterday at 11:36 PM
-- Fix Jedi attributes baseline
addCommand("fixjedi", FixJediCommand)
add this to the commands lua
Ben (stormos Dev)
[GEN2]
 — Yesterday at 11:37 PM
ok
that actually will require a restart
Zubzy — Yesterday at 11:37 PM
ye ajust notice hold up ebfore u dop i have one more file to send
Ben (stormos Dev)
[GEN2]
 — Yesterday at 11:37 PM
ya
Zubzy — Yesterday at 11:37 PM
the command will be /fixjedi name
Ben (stormos Dev)
[GEN2]
 — Yesterday at 11:38 PM
ya
Zubzy — Yesterday at 11:38 PM
and should do all the changes instantly
Ben (stormos Dev)
[GEN2]
 — Yesterday at 11:38 PM
ya
Zubzy — Yesterday at 11:38 PM
/*
    SWGEmu Core3 - FixJediCommand
    Sets Jedi baseline HAM + secondary attributes for a target or self.

    Usage:
      /fixjedi
Expand
FixJediCommand.cpp
4 KB
needs to go in src/server/zone/objects/creature/commands/
Ben (stormos Dev)
[GEN2]
 — Yesterday at 11:39 PM
ya
we need a .lua for this to go in commands.lua
also
Zubzy — Yesterday at 11:40 PM
ok
CommandConfigManager.cpp do u have this file? dont send
Ben (stormos Dev)
[GEN2]
 — Yesterday at 11:41 PM
ya
Zubzy — Yesterday at 11:41 PM
REGISTER_COMMAND(FixJediCommand, "fixjedi");
 make sure this is added
Ben (stormos Dev)
[GEN2]
 — Yesterday at 11:41 PM
ok so dont add it via lua?
Zubzy — Yesterday at 11:41 PM
shouldnt have tp
but got one more file for u
Ben (stormos Dev)
[GEN2]
 — Yesterday at 11:41 PM
ya
Zubzy — Yesterday at 11:42 PM
this pairs with cpp
/*
    Copyright <SWGEmu>
    See file COPYING for copying conditions.
*/

#ifndef FIXJEDICOMMAND_H_
Expand
FixJediCommand.h
1 KB
-- File: scripts/commands/fixjedi.lua

FixJediCommand = {
    name = "fixjedi",
    -- who can use it (0 = player, 1 = CSR, 2 = Dev, 15 = Admin)
    permissionLevel = 15,

    -- syntax shown when someone uses /help fixjedi
    helpString = "Fix Jedi HAM attributes for a target player or self",
    requiresTarget = true
}

AddCommand(FixJediCommand)
 this is for the lua
Ben (stormos Dev)
[GEN2]
 — Yesterday at 11:42 PM
ya
Zubzy — Yesterday at 11:42 PM
fixjedi.lua
bout to send lua
Ben (stormos Dev)
[GEN2]
 — Yesterday at 11:42 PM
ok where do we want the stuff in commandconfigmanager
Zubzy — Yesterday at 11:43 PM
-- File: scripts/commands/fixjedi.lua

FixJediCommand = {
    name = "fixjedi",
    -- who can use it (0 = player, 1 = CSR, 2 = Dev, 15 = Admin)
    permissionLevel = 15,
Expand
fixjedi.lua
1 KB
checking
sry this takeing so long didnt know those attribute scommands want in
Ben (stormos Dev)
[GEN2]
 — Yesterday at 11:44 PM
oh
Zubzy — Yesterday at 11:45 PM
Open server/zone/managers/command/CommandConfigManager.cpp and add entries where other commands are set up (usually in loadDefaultConfig() / registerDefaults() or similar). Use the same helper your file uses for others—examples below cover the common patterns:

Pattern A (string role helper):

setDefaultEnabled("fixjedi", true);
setDefaultPermission("fixjedi", PermissionLevelList::instance()->getLevelNumber("admin"));
// or: setDefaultPermission("fixjedi", 9);


Pattern B (single add/register call):

registerCommand("fixjedi", /enabled/ true, /minLevel/ 9);


Pattern C (map-style):

CommandConfig cfg;
cfg.enabled = true;
cfg.level = 9; // or PermissionLevelList::instance()->getLevelNumber("admin")
commandMap.put("fixjedi", cfg);


Place your lines next to where other built-in admin commands (like setexperience, addskill, etc.) are configured so they’re loaded together
send me the file ill add it in
Ben (stormos Dev)
[GEN2]
 — Yesterday at 11:45 PM
https://github.com/bfitzgit23/mtgserver/blob/update-wip/MMOCoreORB/src/server/zone/managers/objectcontroller/command/CommandConfigManager.cpp
GitHub
mtgserver/MMOCoreORB/src/server/zone/managers/objectcontroller/comm...
Contribute to bfitzgit23/mtgserver development by creating an account on GitHub.
Contribute to bfitzgit23/mtgserver development by creating an account on GitHub.
Zubzy — Yesterday at 11:53 PM
almost done having to manualy add everything in
Ben (stormos Dev)
[GEN2]
 — Yesterday at 11:53 PM
ya
Zubzy — Yesterday at 11:53 PM
sry buf for wait
Ben (stormos Dev)
[GEN2]
 — Yesterday at 11:53 PM
np
Zubzy — Yesterday at 11:55 PM
A) Keep the include at the top (this is correct)
#include "server/zone/objects/creature/commands/FixJediCommand.h"

B) In registerCommands() add the factory registration only

Put it near the other commandFactory.registerCommand<...> lines:

void CommandConfigManager::registerCommands() {
    registerCommands2();
    registerCommands3();
    registerCommands4();

    // Register the C++ class so the factory can build it
    commandFactory.registerCommand<FixJediCommand>(String("fixjedi").toLowerCase());

    // Space Commands
    commandFactory.registerCommand<CommCommand>(String("comm").toLowerCase());
    ...
}


Remove the createCommand("fixjedi") block from registerCommands().

C) In registerSpecialCommands(...) create the slash command entry

Put this right after the /logout bit (same place those “special” commands are created):

void CommandConfigManager::registerSpecialCommands(CommandList* sCommands) {
    info(true) << "Loading Special Commands...";
    slashCommands = sCommands;

    QueueCommand* admin = new AdminCommand("admin", server);
    slashCommands->put(admin);

    QueueCommand* slashCommand = createCommand(String("logout").toLowerCase());
    if (slashCommand == nullptr) {
        error("Could not create command /logout");
    }

    // NOW add /fixjedi here (slashCommands is valid here)
    QueueCommand* fixJediSlash = createCommand(String("fixjedi").toLowerCase());
    if (fixJediSlash == nullptr) {
        error("Could not create command /fixjedi");
    }
    // Optional grouping:
    // fixJediSlash->setCommandGroup(0xe1c9a54a);

    ...
}


That’s all you need. Rebuild the core and restart the zone server. Then in-game, as an admin:
/fixjedi to apply the new HAM to yourself, or /fixjedi SomeName to target another player.
bot to send ifle
Ben (stormos Dev)
[GEN2]
 — Yesterday at 11:57 PM
ya
Zubzy — Yesterday at 11:58 PM
/*
Copyright <SWGEmu>
See file COPYING for copying conditions. */

#include "CommandConfigManager.h"
#include "server/zone/objects/creature/commands/FixJediCommand.h"
Expand
CommandConfigManager (1).cpp
43 KB
Ok afk bio break file u update
Ben (stormos Dev)
[GEN2]
 — 12:00 AM
i did it has errors
Zubzy — 12:00 AM
What was error
Ben (stormos Dev)
[GEN2]
 — 12:00 AM
Image
Zubzy — 12:00 AM
Man never easy is it lol ok one aec
Ben (stormos Dev)
[GEN2]
 — 12:00 AM
lol nope
Zubzy — 12:02 AM
Ok fixing now
Ben (stormos Dev)
[GEN2]
 — 12:02 AM
ok
brb gettin a coke
Zubzy — 12:04 AM
Ok .. I feel bad makein wait .. I wonder if we can spawn in a few nge house as a gift
Ben (stormos Dev)
[GEN2]
 — 12:07 AM
we should
Zubzy — 12:08 AM
wile im patch these two files wanna see if u can spawn on into ur inventory
Ben (stormos Dev)
[GEN2]
 — 12:08 AM
ya ill check
Zubzy — 12:08 AM
ok
Ben (stormos Dev)
[GEN2]
 — 12:08 AM
i brought it down but once up ill check
Zubzy — 12:08 AM
ok
Ben (stormos Dev)
[GEN2]
 — 12:09 AM
hmm the mustafarian bunker isnt in the mtg codde
Zubzy — 12:10 AM
/*
    Copyright <SWGEmu>
    See file COPYING for copying conditions.
*/

#ifndef FIXJEDICOMMAND_H_
Expand
FixJediCommand.h
1 KB
/*
    SWGEmu Core3 - FixJediCommand
    Sets Jedi baseline HAM + secondary attributes for a target or self.

    Usage:
      /fixjedi
Expand
FixJediCommand.cpp
4 KB
alright do u see any other nge hosue listed
﻿
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

    int doQueueCommand(CreatureObject* creature, const uint64& /*targetID*/, const UnicodeString& arguments) const override {
        if (creature == nullptr || !creature->isPlayerCreature())
            return GENERALERROR;

        // Permission check: admin level >= 9 (GM)
        PlayerObject* adminGhost = creature->getPlayerObject();
        if (adminGhost == nullptr || adminGhost->getAdminLevel() < 9) {
            creature->sendSystemMessage("You do not have permission to use /fixjedi.");
            return INVALIDPARAMETERS;
        }

        // Resolve target: default self, or provided first name (case-insensitive without equalsIgnoreCase)
        String arg = arguments.toString().trim();
        bool useSelf = arg.isEmpty() || arg == "self" || arg == "Self" || arg == "SELF";

        CreatureObject* target = creature;

        if (!useSelf) {
            ZoneServer* zoneServer = server->getZoneServer();
            if (zoneServer == nullptr) {
                creature->sendSystemMessage("Server error (ZoneServer is null).");
                return GENERALERROR;
            }

            PlayerManager* pm = zoneServer->getPlayerManager();
            if (pm == nullptr) {
                creature->sendSystemMessage("Server error (PlayerManager is null).");
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

        // Require Jedi flag
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

        String who = target->getFirstName();
        target->sendSystemMessage("Your Jedi baseline attributes have been set.");
        if (target != creature)
            creature->sendSystemMessage("Applied Jedi baseline to " + who + ".");

        return SUCCESS;
    }
};

// Optional factory if your command system uses C++ factories instead of pure Lua binding.
// If not used, it won't hurt to keep it here.
static QueueCommand* FixJediCommandFactory(const String& name, ZoneProcessServer* server) {
    return new FixJediCommand(name, server);
}
FixJediCommand.cpp
4 KB
