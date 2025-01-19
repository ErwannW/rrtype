levelConfig = {
    duration = 300,
    background = {
        {spriteID = "TekkenStage03", speed = 0},
    },
    music = "TekkenSongStage",
    spawns = {
    }
}

local elapsedTime = 0

function isLevelActive()
    return elapsedTime < levelConfig.duration
end

function updateLevel(deltaTime)
    elapsedTime = elapsedTime + deltaTime
    return nil
end
