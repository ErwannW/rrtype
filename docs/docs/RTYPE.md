# R-Type Level Creation Guide

This guide explains how to create a custom level for **R-Type** by modifying several configuration files written in **LUA**.

---

## Step 1: Create a Level Configuration File

1. **Create a LUA file**:
    - Name your file according to your level (e.g., `level_1.lua`) and place it in the `LevelConfiguration` folder.

2. **Add the required functions** in this file:

    - **`isLevelActive`**:  
      This function must **return `true` as long as the level is active** (not completed).  
      Example:
      ```lua
      function isLevelActive()
          return true -- Returns true while the level is ongoing
      end
      ```

    - **`updateLevel`**:  
      This function takes an integer as an argument (the elapsed time since the last call) and returns the following parameters for the entities:
        - Enemy type
        - Movement type (`linear` or `sinusoidal`)
        - Projectile type (or `none` if the entity does not use projectiles)
        - **x** and **y** spawn positions

      Example:
      ```lua
      function updateLevel(elapsedTime)
          return "enemy_type_1", "linear", "fire_1_shoot", 100, 200
      end
      ```

---

## Step 2: Configure Enemies

Edit the **`GameConf.lua`** file, in the `enemy_configuration` section, add the enemy types you want to use.

 ```lua
 enemy_type_1 = {
     hp = 20,
     speed = 10,
     sprite_id = "enemy_type_1",
     destruction_id = "destruction_1",
     heatBoxSize = 10,
     heatBoxType = "circle",
     isBoss = false
 },
 ```

---

## Step 3: Configure Projectiles

In **`GameConf.lua`**, edit the `projectile_configuration` section to define the types of projectiles.

   ```lua
   fire_1_shoot = {
       damage = 10,
       sprite_id = "fire_shoot_1",
   },
```
---

## step 4: Link level to GameConf

In **`GameConf.lua`**, adding your level name in `stageName` like this:

```lua
stageName = {
    "stage01",
    "stage02"
}
```

## Step 5: Add Sprites
1. Edit the **`Assets/Sprite.lua`** file to add the sprites for your new entities:

    ```lua
    spaceship_green = {
        path = "/Sprite/space-ship.png",
        animationFrameRate = 0.02,
        frames = {
            { x = 0, y = 34, width = 33, height = 17 },
            { x = 33, y = 34, width = 33, height = 17 },
            { x = 66, y = 34, width = 33, height = 17 },
            { x = 99, y = 34, width = 33, height = 17 },
            { x = 132, y = 34, width = 33, height = 17 },
        }
    },
    ````
2. If the entity has no animation, use a single frame and set animationFrameRate to 0:
    ```lua
    enemy_type_1 = {
        path = "/Sprite/enemy.png",
        animationFrameRate = 0,
        frames = {
            { x = 0, y = 0, width = 32, height = 32 }
        }
    },
    ```
    In the case the sprite is either a background or a paralax to be displayed in front of the background, the name of the sprite should follow this format:
    ```Game_background_[stage]``` or ```Game_background_[stage]_paralax```
    where [stage] is the name of the stage the background should be displayed.
---

## Summary of Files to Edit:
1. **`LevelConfiguration/level_X.lua`**: Create the level file with the isLevelActive and updateLevel functions.
2. **`LevelConfiguration/GameConf.lua`**: Add configurations for:
   Enemy types (enemy_configuration)
   Projectiles (projectile_configuration)
3. **`Assets/Sprite.lua`**: Add the sprites for your entities and projectiles.