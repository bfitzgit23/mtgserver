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
