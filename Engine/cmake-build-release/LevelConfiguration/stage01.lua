 levelConfig = {
        duration = 20,
        background = {
            {spriteID = "paralax_stage01_1", speed = 20},
            {spriteID = "paralax_stage01_2", speed = 30}
        },
        music = "Level1Sound",
        spawns = {
            {time = 5, type = "enemy_type_1", movement = "linear", projectile = "none", x = 110, y = 50},
            {time = 6, type = "enemy_type_1", movement = "linear", projectile = "none", x = 110, y = 60},
            {time = 7, type = "enemy_type_1", movement = "linear", projectile = "none", x = 110, y = 80},
            {time = 7, type = "enemy_type_1", movement = "linear", projectile = "none", x = 110, y = 15},
            {time = 10, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 110, y = 20},
            {time = 10.1, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 110, y = 23},
            {time = 10.2, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 110, y = 26},
            {time = 10.3, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 110, y = 29},
            {time = 20, type = "enemy_type_1", movement = "linear", projectile = "none", x = 110, y = 30},
            {time = 22, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 110, y = 80},
            {time = 22.1, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 110, y = 83},
            {time = 22.2, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 110, y = 86},
            {time = 30, type = "enemy_type_1", movement = "linear", projectile = "none", x = 110, y = 15},
            {time = 33, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 110, y = 27},
            {time = 35, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 110, y = 40},
            {time = 38, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 110, y = 1},
            {time = 40, type = "enemy_type_1", movement = "linear", projectile = "none", x = 110, y = 57},
            {time = 42, type = "enemy_type_1", movement = "linear", projectile = "none", x = 110, y = 33},
            {time = 45, type = "enemy_type_1", movement = "linear", projectile = "none", x = 110, y = 86},
            {time = 50, type = "enemy_type_1", movement = "linear", projectile = "none", x = 110, y = 15},
            {time = 60, type = "bose_type_1", movement = "linear", projectile = "none", x = 110, y = 60},
            {time = 62, type = "enemy_type_1", movement = "linear", projectile = "none", x = 70, y = 65},
            {time = 62.5, type = "enemy_type_1", movement = "linear", projectile = "none", x = 70, y = 68},
            {time = 63, type = "enemy_type_1", movement = "linear", projectile = "none", x = 70, y = 65},
            {time = 63.5, type = "enemy_type_1", movement = "linear", projectile = "none", x = 70, y = 65},
            {time = 64, type = "enemy_type_1", movement = "linear", projectile = "none", x = 70, y = 68},
            {time = 68, type = "enemy_type_1", movement = "linear", projectile = "none", x = 70, y = 60},
            {time = 68.6, type = "enemy_type_1", movement = "linear", projectile = "none", x = 70, y = 65},
            {time = 70, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 70, y = 65},
            {time = 71, type = "enemy_type_1", movement = "linear", projectile = "none", x = 70, y = 65},
            {time = 72, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 70, y = 65}
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
