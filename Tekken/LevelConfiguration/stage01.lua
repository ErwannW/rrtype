levelConfig = {
    duration = 300,
    background = {
        {spriteID = "TekkenSpriteStage01", speed = 0},
    },
    music = "TekkenSongStage",
    spawns = {
    }
}

local elapsedTime = 0
local spawnIndex = 1

function isLevelActive()
    return elapsedTime < levelConfig.duration
end

function updateLevel(deltaTime)
    elapsedTime = elapsedTime + deltaTime
    return nil
end
