 levelConfig = {
        duration = 15,
        background = {
            {spriteID = "paralax_stage02_1", speed = 10},
            {spriteID = "paralax_stage02_2", speed = 15}
        },
        music = "Level2Sound",
        spawns = {
            {time = 1, type = "enemy_type_2", movement = "fix_after_pop_up", projectile = "none", x = 40, y = 120},
            {time = 4, type = "enemy_type_3", movement = "enemy_type_3", projectile = "none", x = 40, y = 92},
            {time = 4.5, type = "enemy_type_3", movement = "enemy_type_3", projectile = "none", x = 40, y = 92},
            {time = 5, type = "enemy_type_3", movement = "enemy_type_3", projectile = "none", x = 40, y = 92},
            {time = 5.5, type = "enemy_type_3", movement = "enemy_type_3", projectile = "none", x = 40, y = 92},
            {time = 5.8, type = "enemy_type_3", movement = "enemy_type_3", projectile = "none", x = 40, y = 92},


            {time = 7, type = "enemy_type_2", movement = "fix_after_pop_up", projectile = "none", x = 20, y = 120},
            {time = 7.5, type = "enemy_type_3", movement = "enemy_type_3", projectile = "none", x = 20, y = 92},
            {time = 7.8, type = "enemy_type_3", movement = "enemy_type_3", projectile = "none", x = 20, y = 92},
            {time = 8, type = "enemy_type_3", movement = "enemy_type_3", projectile = "none", x = 20, y = 92},
            {time = 8.3, type = "enemy_type_3", movement = "enemy_type_3", projectile = "none", x = 20, y = 92},
            {time = 8.5, type = "enemy_type_3", movement = "enemy_type_3", projectile = "none", x = 20, y = 92},

            {time = 6, type = "boss_type_2", movement = "linear", projectile = "projectile_bose_2_creation", x = 110, y = 50}
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