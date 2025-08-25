-- One-time Jedi baseline fixer on login

JediBaselineFix = ScreenPlay:new {
    numberOfActs = 1
}

registerScreenPlay("JediBaselineFix", true)

function JediBaselineFix:start()
    -- nothing to do at server start
end

function JediBaselineFix:onPlayerLoggedIn(pPlayer)
    if pPlayer == nil then return end

    local player = CreatureObject(pPlayer)
    if player == nil or (not player:isPlayerCreature()) then return end

    -- already applied? bail
    if player:hasObjVar("jedi.baselineFix1") then return end

    -- robust Jedi detection: skills (works across forks)
    local isJedi =
        player:hasSkill("force_title_jedi_novice") or
        player:hasSkill("jedi_padawan") or
        player:hasSkill("force_sensitive_novice") or
        player:hasSkill("force_discipline_light_saber_novice")

    if not isJedi then return end

    -- try to set jediState if your fork exposes it (safe-guarded)
    local ghost = player:getPlayerObject()
    if ghost ~= nil and ghost.setJediState ~= nil then
        pcall(function() ghost:setJediState(4) end)
    end

    -- target baseline (HAM pools order 0..8)
    local target = {1100, 900, 650, 600, 600, 500, 500, 450, 450}

    -- bump any pool that's below target
    for i = 0, 8 do
        if player:getMaxHAM(i) < target[i+1] then
            player:setBaseHAM(i, target[i+1])
            player:setHAM(i,      target[i+1])
            player:setMaxHAM(i,   target[i+1])
        end
    end

    -- mark as done (idempotent)
    player:setObjVar("jedi.baselineFix1", 1)

    -- optional: let them know
    player:sendSystemMessage("Jedi baseline stats applied.")
end
