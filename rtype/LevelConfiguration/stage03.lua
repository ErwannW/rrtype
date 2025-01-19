math.randomseed(os.time())

levelConfig = {
    duration = 25,
    background = {
        {spriteID = "paralax_stage02_1", speed = 10},
        {spriteID = "paralax_stage04_2", speed = 15}
    },
    music = "Level4Sound",
    spawns = {}
}

local elapsedTime = 0
local spawnIndex = 1
local lastSpawnTime = 0

function isLevelActive()
    return elapsedTime < levelConfig.duration
end

function updateLevel(deltaTime)
    elapsedTime = elapsedTime + deltaTime

    if elapsedTime >= levelConfig.duration - 5 and not levelConfig.forceBossSpawn then
        local spawn = {
            time = elapsedTime,
            type = "boss_type_1",
            movement = "linear",
            projectile = "none",
            x = 110,
            y = 60,
        }

        table.insert(levelConfig.spawns, spawn)
        levelConfig.forceBossSpawn = true
    end

    if elapsedTime - lastSpawnTime >= math.random(1, 4) * 0.5 then
        local spawnTime = elapsedTime + math.random(1, 4) * 0.5

        local spawn = {
            time = spawnTime,
            type = "enemy_type_" .. math.random(1),
            movement = "linear",
            projectile = "none",
            x = 110,
            y = math.random(10, 90)
        }

        table.insert(levelConfig.spawns, spawn)
        lastSpawnTime = elapsedTime
    end

    if spawnIndex <= #levelConfig.spawns and elapsedTime >= levelConfig.spawns[spawnIndex].time then
        local spawn = levelConfig.spawns[spawnIndex]
        spawnIndex = spawnIndex + 1
        return spawn
    end

    return nil
end
