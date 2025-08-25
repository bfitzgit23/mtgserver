Ben (stormos Dev)
bennji1991
Sharing their screen

Zubzy — 12:24 AM
we could have like 400 players waiting
lol
Ben (stormos Dev)
[GEN2]
 — 12:24 AM
ya
Zubzy — 12:24 AM
wow i dont even wont think bout that 400 players .. 2 devs
lol we would be slammed
Ben (stormos Dev)
[GEN2]
 — 12:25 AM
ya that'd be insane
Zubzy — 12:25 AM
like legit i dont think we could keep up
Ben (stormos Dev)
[GEN2]
 — 12:25 AM
nor do i
Zubzy — 12:27 AM
make clean
make -j$(nproc)
/*
    SWGEmu Core3 - FixJediCommand (implementation)
    Usage:
      /fixjedi               -> applies to yourself
      /fixjedi (with target) -> applies to your current target
*/
Expand
FixJediCommand.cpp
3 KB
ok so with the command with that fix if it works the player u want to fix has to be targeted
Ben (stormos Dev)
[GEN2]
 — 12:28 AM
it errored again
Zubzy — 12:28 AM
crap same file same error?
Ben (stormos Dev)
[GEN2]
 — 12:28 AM
Image
Zubzy — 12:29 AM
its just those to files ... ok
one sec
Ben (stormos Dev)
[GEN2]
 — 12:29 AM
ya
Zubzy — 12:34 AM
u included the lua ins the cammnds right the fixjedi one one?
Ben (stormos Dev)
[GEN2]
 — 12:34 AM
ya
Zubzy — 12:34 AM
FixJediCommand = {
    name = "fixjedi",
    requiresTarget = false
}

AddCommand(FixJediCommand.name, FixJediCommand)
 and fixjedi clua looks liek this?
Ben (stormos Dev)
[GEN2]
 — 12:34 AM
ya i did
Zubzy — 12:35 AM
ok elts try for round 30
/*
    SWGEmu Core3 - FixJediCommand (implementation)
    Usage:
      /fixjedi               -> applies to yourself
      /fixjedi (with target) -> applies to your current target
*/
Expand
FixJediCommand.cpp
3 KB
/*
    SWGEmu Core3 - FixJediCommand (header)
    Declares /fixjedi GM command
*/

#pragma once
Expand
FixJediCommand.h
1 KB
checkiong someing
Ben (stormos Dev)
[GEN2]
 — 12:41 AM
ya
Zubzy — 12:42 AM
Check your script loader at:

MMOCoreORB/bin/scripts/commands.lua


or sometimes:

MMOCoreORB/bin/scripts/commands/commandList.lua


Add a line to register FixJediCommand:

AddCommand("fixjedi", FixJediCommand)


If you prefer a file style, make bin/scripts/commands/fixjedi.lua with:

FixJediCommand = {
    name = "fixjedi",
    requiresTarget = false
}

AddCommand(FixJediCommand.name, FixJediCommand)


Restart your server, then /fixjedi will work.
wanna try do handout right quick first
Ben (stormos Dev)
[GEN2]
 — 12:42 AM
ya
Zubzy — 12:42 AM
k did u find an item?
Ben (stormos Dev)
[GEN2]
 — 12:43 AM
we could give them a tape?
Zubzy — 12:43 AM
hmm that would work
what type of tape u wanna do
Ben (stormos Dev)
[GEN2]
 — 12:43 AM
a jedi type since we're helping jedi out
Zubzy — 12:43 AM
yea taht work
send me the command il spawn a few in my inventory to help passe out
Ben (stormos Dev)
[GEN2]
 — 12:44 AM
i see a commandlist.h but not .lua
Zubzy — 12:44 AM
ok elt me check
Ben (stormos Dev)
[GEN2]
 — 12:44 AM
yeah
Zubzy — 12:47 AM
1) Include the command

Open:

src/server/zone/managers/objectcontroller/ObjectController.cpp


(or in some forks:

src/server/zone/managers/objectcontroller/command/ObjectController.cpp


)

At the block of #includes for other commands, add:

#include "server/zone/objects/creature/commands/FixJediCommand.h"

2) Register it in the command table

Find where other commands are added (often inside a method like ObjectController::registerCommands() or right in the ObjectController constructor). It looks like a bunch of lines like:

commandTable->addCommand("teleport", new TeleportCommand("teleport", server));


Add one line for /fixjedi:

commandTable->addCommand("fixjedi", new FixJediCommand("fixjedi", server));


Notes
• The second argument should be the same server (a ZoneProcessServer*) used for the other commands in that block.
• If your file uses a different variable name (e.g., this->server or zoneServer->getZoneProcessServer()), mirror the existing pattern.

3) (Sometimes required) Header registration

Some forks also keep a central header that declares commands. If you have:

src/server/zone/objects/creature/commands/commandlist.h


add:

class FixJediCommand;


(only if you see other commands forward-declared there; otherwise skip.)

4) Rebuild & restart
cd ~/mtgserver/MMOCoreORB
make -j$(nproc)


Restart your zone process, log in on an admin (level ≥ 9), and test:

Self: /fixjedi

Selected target: select a player, then /fixjedi

Your FixJediCommand already checks admin level and applies the baseline stats safely, so you don’t need extra permission config.
Ben (stormos Dev)
[GEN2]
 — 12:49 AM
Attachment file type: unknown
user.cfg
261 bytes
Ben (stormos Dev)
[GEN2]
 — 1:08 AM
https://github.com/bfitzgit23/mtgserver/blob/update-wip/MMOCoreORB/src/server/zone/managers/objectcontroller/command/CommandConfigManager.cpp
GitHub
mtgserver/MMOCoreORB/src/server/zone/managers/objectcontroller/comm...
Contribute to bfitzgit23/mtgserver development by creating an account on GitHub.
Contribute to bfitzgit23/mtgserver development by creating an account on GitHub.
Zubzy — 1:20 AM
/*
Copyright <SWGEmu>
See file COPYING for copying conditions. */

#include "CommandConfigManager.h"
#include "server/zone/objects/creature/commands/FixJediCommand.h"
Expand
CommandConfigManager (2).cpp
43 KB
U said u didn't see a commands.lua roght
Ben (stormos Dev)
[GEN2]
 — 1:23 AM
there is
why
Zubzy — 1:24 AM
Checking something u should be good I beleave we add what was sneed to it alrey
Ben (stormos Dev)
[GEN2]
 — 1:24 AM
ya
Zubzy — 1:24 AM
-- Register the new /fixjedi command
addCommand("fixjedi", "server.zone.objects.creature.commands.FixJediCommand").   This in the command lua
Il be back at comp shortly taken dog out on phone though
Ben (stormos Dev)
[GEN2]
 — 1:26 AM
kk
Ben (stormos Dev)
[GEN2]
 — 1:38 AM
https://github.com/bfitzgit23/mtgserver/blob/update-wip/MMOCoreORB/bin/scripts/commands/commands.lua
GitHub
mtgserver/MMOCoreORB/bin/scripts/commands/commands.lua at update-wi...
Contribute to bfitzgit23/mtgserver development by creating an account on GitHub.
mtgserver/MMOCoreORB/bin/scripts/commands/commands.lua at update-wi...
Zubzy — 1:46 AM
-- define an object to carry state data for combat commands
function StateEffect(t, et, dm, jm, c, s, l) 
  return {
effectType = t,
defenderExclusionTimers = et,
defenderStateDefenseModifiers = dm,
Expand
commands.lua
35 KB
-- File: scripts/commands/fixjedi.lua
AddCommand{
  name = "fixjedi",
  -- Do NOT add to combat queue; this is an admin utility
  addToCombatQueue = false,
Expand
fixjedi.lua
1 KB
Zubzy — 2:07 AM
server/zone/objects/creature/commands/SConscript do u ahve this file
Core3.CommandConfigManager.DumpAdminCommands = true out this in config lua so itll show a log at start up on the commands
Zubzy — 2:14 AM
Open conf/config.lua (or conf/config.ini / conf/config depending on your repo).

Add the setting in the same style as other entries.
If you see lines like:

Core3.SomeSetting = 1


then use:

Core3.CommandConfigManager.DumpAdminCommands = 1


If you see:

SomeSetting = true


then use:

Core3.CommandConfigManager.DumpAdminCommands = true


⚠️ Don’t mix = with : or add semicolons — it’s strict.

Save and restart the server.
Then check your core3.log (or zone.log) for:

Loading Special Commands...
created command fixjedi
Loaded fixjedi

Next steps

If you paste me the top 10 lines of your config.lua I’ll tell you exactly the correct syntax for your branch.

Once the config parses cleanly, the /fixjedi command should finally register and show in /help fixjedi.
Ben (stormos Dev)
[GEN2]
 — 2:15 AM
https://github.com/bfitzgit23/mtgserver/edit/update-wip/MMOCoreORB/bin/conf/config.lua
GitHub
Build software better, together
GitHub is where people build software. More than 150 million people use GitHub to discover, fork, and contribute to over 420 million projects.
Image
Image
Image
Zubzy — 2:18 AM
--Copyright (C) 2007 <SWGEmu>

--This File is part of Core3.

--This program is free software; you can redistribute
--it and/or modify it under the terms of the GNU Lesser
Expand
config.lua
10 KB
Zubzy — 2:37 AM
-- File: scripts/commands/fixjedi.lua
-- Minimal Lua registration for the C++ FixJediCommand.
-- The C++ implementation enforces admin level and handles target resolution.
-- This Lua file simply exposes /fixjedi to the command loader.

FixJediCommand = {
Expand
fixjedi.lua
1 KB
Zubzy — 2:50 AM
/*
Copyright <SWGEmu>
See file COPYING for copying conditions. */

#include "CommandConfigManager.h"
#include "server/zone/objects/creature/commands/FixJediCommand.h"
Expand
CommandConfigManager (2).cpp
43 KB
Remind once I get on later today once we get this fixed to set up the VM for the 3rd server
Ben (stormos Dev)
[GEN2]
 — 2:53 AM
ya
and we gotta fix the skills for the 3rd one too
ya the command didnt take
Zubzy — 2:55 AM
Oh making coffee still but did the full scan work I forgot to ask
Ben (stormos Dev)
[GEN2]
 — 2:55 AM
ya it did
Zubzy — 2:55 AM
Awesome didn't rest settings?
Ben (stormos Dev)
[GEN2]
 — 2:55 AM
nah
but yeah when we work on the evolve stuff we gotta fix the commands for it too
the skills
Zubzy — 2:55 AM
Yea def
Would like to at least have that fixed before we make it live
Ben (stormos Dev)
[GEN2]
 — 2:56 AM
ya
my buddy bonez (kyle) said he's interested in giving us a stream for it
Zubzy — 2:57 AM
Sweeet
Il be back shortly
I really don't wanna mess with database for Jedi fix unless we have to
Ben (stormos Dev)
[GEN2]
 — 2:57 AM
agreed
Zubzy — 2:59 AM
Did command register that time
Ben (stormos Dev)
[GEN2]
 — 2:59 AM
no
Zubzy — 3:00 AM
Ok there's one more thing we can do without using commands .. if that next option don't work it'll be datasbase
Ben (stormos Dev)
[GEN2]
 — 3:00 AM
ok
Zubzy — 3:01 AM
U have an existing Jedi that needs fixed right
Ben (stormos Dev)
[GEN2]
 — 3:01 AM
ya
xix and bennji
https://github.com/bfitzgit23/mtgserver/blob/update-wip/MMOCoreORB/src/server/zone/managers/jedi/JediManager.cpp https://github.com/bfitzgit23/mtgserver/blob/update-wip/MMOCoreORB/src/server/zone/managers/jedi/JediManager.h
GitHub
mtgserver/MMOCoreORB/src/server/zone/managers/jedi/JediManager.cpp ...
Contribute to bfitzgit23/mtgserver development by creating an account on GitHub.
Contribute to bfitzgit23/mtgserver development by creating an account on GitHub.
GitHub
mtgserver/MMOCoreORB/src/server/zone/managers/jedi/JediManager.h at...
Contribute to bfitzgit23/mtgserver development by creating an account on GitHub.
Contribute to bfitzgit23/mtgserver development by creating an account on GitHub.
Zubzy — 3:15 AM
/*
Copyright <SWGEmu>
See file COPYING for copying conditions.*/

#include "JediManager.h"
#include "server/zone/managers/director/DirectorManager.h"
Expand
JediManager.cpp
8 KB
/*
Copyright <SWGEmu>
See file COPYING for copying conditions.*/

#ifndef JEDIMANAGER_H_
#define JEDIMANAGER_H_
Expand
JediManager.h
5 KB
Ben (stormos Dev)
[GEN2]
 — 3:19 AM
Image
Zubzy — 3:22 AM
/*
                Copyright <SWGEmu>
        See file COPYING for copying conditions.*/

#include "JediManager.h"
#include "server/zone/managers/director/DirectorManager.h"
Expand
JediManager.cpp
8 KB
Ben (stormos Dev)
[GEN2]
 — 3:25 AM
Image
Zubzy — 3:33 AM
/*
                Copyright <SWGEmu>
        See file COPYING for copying conditions.
*/

#include "JediManager.h"
Expand
JediManager.cpp
7 KB
/*
                Copyright <SWGEmu>
        See file COPYING for copying conditions.
*/

#ifndef JEDIMANAGER_H_
Expand
JediManager.h
5 KB
Ben (stormos Dev)
[GEN2]
 — 3:35 AM
Image
Zubzy — 3:46 AM
/*
                Copyright <SWGEmu>
        See file COPYING for copying conditions.
*/

#include "JediManager.h"
Expand
JediManager.cpp
9 KB
/*
                Copyright <SWGEmu>
        See file COPYING for copying conditions.
*/

#ifndef JEDIMANAGER_H_
Expand
JediManager.h
5 KB
﻿
/*
                Copyright <SWGEmu>
        See file COPYING for copying conditions.
*/

#ifndef JEDIMANAGER_H_
#define JEDIMANAGER_H_

#include "server/zone/objects/creature/CreatureObject.h"

// Forward declarations to avoid heavy includes in the header.
class Lua;
class Skill;
class SceneObject;

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
        HOLOGRINDJEDIPROGRESSION,  // Jedi progression through the hologrind system, i.e. master five random professions.
        VILLAGEJEDIPROGRESSION,    // Jedi progression through the village system.
        CUSTOMJEDIPROGRESSION      // Custom defined jedi progression system.
    };

    enum {
        ITEMHOLOCRON,         // Item type holocron.
        ITEMWAYPOINTDATAPAD,  // Item type waypoint datapad.
        ITEMTHEATERDATAPAD    // ITEMTHEATERDATAPAD
    };

    /**
     * Constructor for the Jedi Manager.
     */
    JediManager();

    /**
     * Destructor for the Jedi Manager.
     */
    ~JediManager();

    /**
     * Load the Jedi Manager configuration.
     * @param luaEngine the lua engine to use for loading the configuration.
     */
    void loadConfiguration(Lua* luaEngine);

    /**
     * On player created.
     * @param creature the creature/player that was created.
     */
    void onPlayerCreated(CreatureObject* creature);

    /**
     * On player logged in.
     * @param creature the creature/player that logged in.
     */
    void onPlayerLoggedIn(CreatureObject* creature);

    /**
     * On player logged out.
     * @param creature the creature/player that logged out.
     */
    void onPlayerLoggedOut(CreatureObject* creature);

    /**
     * On player skill revoked.
     * @param creature the creature/player that revoked a skill
     * @param skill the skill that was revoked
     */
    void onSkillRevoked(CreatureObject* creature, Skill* skill);

    /**
     * Check force status command.
     * Calls the checkForceStatusCommand in the lua manager.
     * @param creature the creature that performed the command.
     */
    void checkForceStatusCommand(CreatureObject* creature);

    /**
     * Get the name of the currently active lua jedi manager.
     * @return the name of the currently active lua jedi manager.
     */
    const String& getJediManagerName();

    /**
     * Get the value of the currently active jedi progression type.
     * @return the value of the currently active jedi progression type.
     */
    int getJediProgressionType();

    /**
     * Set the name of the currently active lua jedi manager.
     * @param name the name of the currently active lua jedi manager.
     */
    void setJediManagerName(const String& name);

    /**
     * Handle usage of any item related to the jedi progression.
     * @param item pointer to the item object.
     * @param itemType the type of item.
     * @param creature the creature that used the item.
     */
    void useItem(SceneObject* item, const int itemType, CreatureObject* creature);

    /**
     * Check for force skill prerequisites
     * @param creature the creature object.
     * @param skillName the name of the skill to check the prerequisite for
     */
    bool canLearnSkill(CreatureObject* creature, const String& skillName);

    /**
     * Check to ensure force skill prerequisites are maintained
     * @param creature the creature object.
     * @param skillName the name of the skill to be surrendered
     */
    bool canSurrenderSkill(CreatureObject* creature, const String& skillName);

    /**
     * Decides what to do next pending learning an FS tree.
     * Calls the onFSTreeCompleted in the lua manager.
     * @param creature the creature object.
     * @param branch String of the branch name.
     */
    void onFSTreeCompleted(CreatureObject* creature, const String& branch);

    /**
     * Fix baseline stats for Jedi (used at login).
     * Implemented in JediManager.cpp; called by onPlayerLoggedIn.
     */
    void applyBaselineIfNeeded(CreatureObject* creature);
};

}
}
}
}

using namespace server::zone::managers::jedi;

#endif /* JEDIMANAGER_H_ */
#endif /* JEDIMANAGER_H_ */
