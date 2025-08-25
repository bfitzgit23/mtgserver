-- File: scripts/commands/fixjedi.lua
AddCommand{
  name = "fixjedi",
  -- Do NOT add to combat queue; this is an admin utility
  addToCombatQueue = false,

  -- Targeting for this admin command is handled in C++ (self or selected)
  -- Use NONE here so the table parses cleanly
  targetType = 0,

  -- Basic defaults (not really used by our handler)
  maxRangeToTarget = 0,
  defaultTime = 0.0,
  defaultPriority = 100,

  -- Optional: prevent obviously invalid locomotions
  invalidLocomotions = "dead,incapacitated"
}
