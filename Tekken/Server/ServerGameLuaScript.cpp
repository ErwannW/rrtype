/*
** EPITECH PROJECT, 2024
** Engine
** File description:
** LevelOne
*/

#include <chrono>
#include <string>

// ENGINE INCLUDES
#include "../../Engine/World.hpp"

// LOCAL INCLUDES
#include "ServerGame.hpp"
#include "ServerLobby.hpp"
#include "../Systems/Systems.h"

extern "C" {
  #include <lua.h>
  #include "lauxlib.h"
  #include "lualib.h"
}

namespace MGE::Server
{

   void ServerGame::loadBackground() {
    lua_getglobal(StageLuaScript, "levelConfig");

    if (!lua_istable(StageLuaScript, -1)) {
      world.engine.GetLogger()->Error("'levelConfig' is not a table");
        lua_pop(StageLuaScript, 1);
        return;
    }

    lua_getfield(StageLuaScript, -1, "music");
    if (lua_isstring(StageLuaScript, -1)) {
        levelMusicID = lua_tostring(StageLuaScript, -1);
    }
    lua_pop(StageLuaScript, 1);

    lua_getfield(StageLuaScript, -1, "background");

    if (!lua_istable(StageLuaScript, -1)) {
        world.engine.GetLogger()->Error("'background' is not a table");
        lua_pop(StageLuaScript, 1);
        return;
    }

    lua_len(StageLuaScript, -1);
    int backgroundCount = lua_tointeger(StageLuaScript, -1);
    lua_pop(StageLuaScript, 1);

    for (int i = 1; i <= backgroundCount; ++i) {

        lua_pushinteger(StageLuaScript, i);
        lua_gettable(StageLuaScript, -2);

        if (!lua_istable(StageLuaScript, -1)) {
            world.engine.GetLogger()->Error("the element is not a table");
            lua_pop(StageLuaScript, 1);
            continue;
        }

        lua_getfield(StageLuaScript, -1, "spriteID");
        RType::BackgroundConfig background;
        if (lua_isstring(StageLuaScript, -1)) {
            background.spriteId = lua_tostring(StageLuaScript, -1);
        }
        lua_pop(StageLuaScript, 1);

        lua_getfield(StageLuaScript, -1, "speed");
        if (lua_isnumber(StageLuaScript, -1)) {
            background.speed = lua_tonumber(StageLuaScript, -1);
        }
        lua_pop(StageLuaScript, 1);

        paralaxConfig.push_back(background);

        lua_pop(StageLuaScript, 1);
    }

    lua_pop(StageLuaScript, 1);

}

    void ServerGame::loadEnemyEntityFromConfig() {

        lua_getglobal(StageLuaScript, "enemy_configuration");
        if (!lua_istable(StageLuaScript, -1)) {
            world.engine.GetLogger()->Error("Error: 'enemy_configuration' is not a table!");
            lua_pop(StageLuaScript, 1);
            return;
        }
        lua_pushnil(StageLuaScript);
        while (lua_next(StageLuaScript, -2)) {
            std::string enemyName = lua_tostring(StageLuaScript, -2);
            if (lua_istable(StageLuaScript, -1)) {
                EnemyConfig config;

                lua_getfield(StageLuaScript, -1, "hp");
                config.Point_of_live = static_cast<int>(lua_tointeger(StageLuaScript, -1));
                lua_pop(StageLuaScript, 1);

                lua_getfield(StageLuaScript, -1, "speed");
                config.speed = static_cast<float>(lua_tonumber(StageLuaScript, -1));
                lua_pop(StageLuaScript, 1);

                lua_getfield(StageLuaScript, -1, "sprite_id");
                config.sprite_id = lua_tostring(StageLuaScript, -1);
                lua_pop(StageLuaScript, 1);

                lua_getfield(StageLuaScript, -1, "destruction_id");
                config.destruction_id = lua_tostring(StageLuaScript, -1);
                lua_pop(StageLuaScript, 1);

                lua_getfield(StageLuaScript, -1, "heatBoxSize");
                config.heatBoxSize = static_cast<int>(lua_tointeger(StageLuaScript, -1));
                lua_pop(StageLuaScript, 1);

                lua_getfield(StageLuaScript, -1, "heatBoxType");
                std::string heatBoxType = lua_tostring(StageLuaScript, -1);
                lua_pop(StageLuaScript, 1);

                if (heatBoxType == "square")
                        config.heatBoxType = HeatBoxType::Square;
                if (heatBoxType == "circle")
                        config.heatBoxType = HeatBoxType::Circle;

                lua_getfield(StageLuaScript, -1, "isBoss");
                config.isBosse = lua_toboolean(StageLuaScript, -1);
                lua_pop(StageLuaScript, 1);

                allEnemyConf[enemyName] = config;
            }
            lua_pop(StageLuaScript, 1);
        }
        lua_pop(StageLuaScript, 1);
    }

    void ServerGame::loadAllLevel() const {
        lua_getglobal(StageLuaScript, "stageName");

        if (!lua_istable(StageLuaScript, -1)) {
            world.engine.GetLogger()->Error("Error: 'stageName' is not a table!");
            lua_pop(StageLuaScript, 1);
            return;
        }

        lua_pushnil(StageLuaScript);
        world.luaFileLevelNames.clear();
        while (lua_next(StageLuaScript, -2)) {
            if (lua_isstring(StageLuaScript, -1)) {
                std::string stageName = lua_tostring(StageLuaScript, -1);
                world.luaFileLevelNames.push_back(stageName);
            }
            lua_pop(StageLuaScript, 1);
        }
        lua_pop(StageLuaScript, 1);
    }

    void ServerGame::loadProjectileEntityFromConfig() {

        lua_getglobal(StageLuaScript, "projectile_configuration");
        if (!lua_istable(StageLuaScript, -1)) {
            world.engine.GetLogger()->Error("Error: 'projectile_configuration' is not a table!");
            lua_pop(StageLuaScript, 1);
            return;
        }

        lua_pushnil(StageLuaScript);
        while (lua_next(StageLuaScript, -2)) {
            std::string projectileName = lua_tostring(StageLuaScript, -2);
            if (lua_istable(StageLuaScript, -1)) {
                ProjectileConfig config;

                lua_getfield(StageLuaScript, -1, "damage");
                config.damage = static_cast<int>(lua_tointeger(StageLuaScript, -1));
                lua_pop(StageLuaScript, 1);

                lua_getfield(StageLuaScript, -1, "sprite_id");
                config.sprite_id = lua_tostring(StageLuaScript, -1);
                lua_pop(StageLuaScript, 1);

                allProjectilesConf[projectileName] = config;
            }
            lua_pop(StageLuaScript, 1);
        }
        lua_pop(StageLuaScript, 1);
    }

    void ServerGame::loadScript(const std::string& filename) {
        if (luaL_dofile(StageLuaScript, filename.c_str())) {
            world.engine.GetLogger()->Error("When we load stageLevel");
            lua_pop(StageLuaScript, 1);
        }
        loadEnemyEntityFromConfig();
        loadProjectileEntityFromConfig();
        loadBackground();
    }

    bool ServerGame::extractSpawnInfo( SpawnInfo& spawn) {
        if (!lua_istable(StageLuaScript, -1)) {
            return false;
        }

        lua_getfield(StageLuaScript, -1, "type");
        spawn.type = lua_tostring(StageLuaScript, -1);
        lua_pop(StageLuaScript, 1);

        lua_getfield(StageLuaScript, -1, "movement");
        spawn.movement = lua_tostring(StageLuaScript, -1);
        lua_pop(StageLuaScript, 1);

        lua_getfield(StageLuaScript, -1, "projectile");
        spawn.projectile = lua_tostring(StageLuaScript, -1);
        lua_pop(StageLuaScript, 1);

        lua_getfield(StageLuaScript, -1, "x");
        spawn.x = static_cast<int>(lua_tointeger(StageLuaScript, -1));
        lua_pop(StageLuaScript, 1);

        lua_getfield(StageLuaScript, -1, "y");
        spawn.y = static_cast<int>(lua_tointeger(StageLuaScript, -1));
        lua_pop(StageLuaScript, 1);

        return true;
    }

    bool ServerGame::callUpdateLevel(double deltaTime, SpawnInfo& spawn) {
        lua_getglobal(StageLuaScript, "updateLevel");
        if (!lua_isfunction(StageLuaScript, -1)) {
            world.engine.GetLogger()->Error("UpdateLevel are not a Lua function");
            return false;
        }

        lua_pushnumber(StageLuaScript, deltaTime);

        if (lua_pcall(StageLuaScript, 1, 1, 0) != LUA_OK) {
            world.engine.GetLogger()->Error("When we call Lua function");
            lua_pop(StageLuaScript, 1);
            return false;
        }

        if (lua_isnil(StageLuaScript, -1)) {
            lua_pop(StageLuaScript, 1);
            return false;
        }

        bool success = extractSpawnInfo(spawn);
        lua_pop(StageLuaScript, 1);
        return success;
    }
}