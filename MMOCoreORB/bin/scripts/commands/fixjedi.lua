-- File: scripts/commands/fixjedi.lua
-- Minimal Lua registration for the C++ FixJediCommand.
-- The C++ implementation enforces admin level and handles target resolution.
-- This Lua file simply exposes /fixjedi to the command loader.

FixJediCommand = {
    name = "fixjedi",
    helpString = "Fix Jedi baseline HAM for yourself or your current target (Admin only)",
    -- Not strictly required, but keeps help text consistent with your other admin tools:
    requiresTarget = false -- target is optional; command will default to self
}

AddCommand(FixJediCommand)
