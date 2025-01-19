 levelConfig = {
        duration = 10,
        background = {
            {spriteID = "paralax_stage02_1", speed = 5},
            {spriteID = "paralax_stage02_2", speed = 10}
        },
        music = "Level2Sound",
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

    if spawnIndex <= #levelConfig.spawns and elapsedTime >= levelConfig.spawns[spawnIndex].time then
        local spawn = levelConfig.spawns[spawnIndex]
        spawnIndex = spawnIndex + 1
        return spawn
    end

    return nil
end