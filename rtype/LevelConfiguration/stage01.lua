 levelConfig = {
        duration = 20,
        background = {
            {spriteID = "paralax_stage01_1", speed = 10},
            {spriteID = "paralax_stage01_2", speed = 15}
        },
        music = "Level1Sound",
        spawns = {
            {time = 1, type = "enemy_type_1", movement = "linear", projectile = "none", x = 110, y = 50},
            {time = 2, type = "enemy_type_1", movement = "linear", projectile = "none", x = 110, y = 60},
            {time = 3, type = "enemy_type_1", movement = "linear", projectile = "none", x = 110, y = 80},
            {time = 4, type = "enemy_type_1", movement = "linear", projectile = "none", x = 110, y = 15},
            {time = 5, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 110, y = 20},
            {time = 5.2, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 110, y = 23},
            {time = 5.4, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 110, y = 26},
            {time = 5.6, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 110, y = 29},
            {time = 5.7, type = "enemy_type_1", movement = "linear", projectile = "none", x = 110, y = 30},
            {time = 5.8, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 110, y = 80},
            {time = 6, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 110, y = 83},
            {time = 6.2, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 110, y = 86},
            {time = 6.4, type = "enemy_type_1", movement = "linear", projectile = "none", x = 110, y = 15},
            {time = 7, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 110, y = 27},
            {time = 7.2, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 110, y = 40},
            {time = 7.4, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 110, y = 1},
            {time = 7.8, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 110, y = 57},
            {time = 7.10, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 110, y = 33},
            {time = 8, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 110, y = 86},
            {time = 8.2, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 110, y = 15},
            {time = 8.4, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 110, y = 60},
            {time = 8.6, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 70, y = 65},
            {time = 9, type = "enemy_type_1", movement = "linear", projectile = "none", x = 70, y = 68},
            {time = 10, type = "enemy_type_1", movement = "linear", projectile = "none", x = 70, y = 65},
            {time = 11, type = "enemy_type_1", movement = "linear", projectile = "none", x = 70, y = 65},
            {time = 12, type = "enemy_type_1", movement = "linear", projectile = "none", x = 70, y = 68},
            {time = 13, type = "enemy_type_1", movement = "linear", projectile = "none", x = 70, y = 60},
            {time = 14, type = "enemy_type_1", movement = "linear", projectile = "none", x = 70, y = 65},
            {time = 15, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 70, y = 65},
            {time = 16, type = "enemy_type_1", movement = "sinusoidal", projectile = "none", x = 70, y = 65},
            {time = 11, type = "bose_type_1", movement = "linear", projectile = "none", x = 110, y = 60}   
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
