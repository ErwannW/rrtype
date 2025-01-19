/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Lua
*/

#include "Lua.hpp"

extern "C" {
    #include <lua.h>
    #include "lauxlib.h"
    #include "lualib.h"
}

/**
 * @brief Constructor of the Lua class
 */
Lua::Lua()
{
    Lua_state = luaL_newstate();

    luaL_openlibs(Lua_state);

    if (luaL_dofile(Lua_state, "Assets/Sprite.lua") != LUA_OK) {
        std::cerr << "ERROR Lua : " << lua_tostring(Lua_state, -1) << std::endl;
        lua_pop(Lua_state, 1);
    }
    if (luaL_dofile(Lua_state, "Assets/Text.lua") != LUA_OK) {
        std::cerr << "ERROR Lua : " << lua_tostring(Lua_state, -1) << std::endl;
        lua_pop(Lua_state, 1);
    }
    if (luaL_dofile(Lua_state, "Assets/Sound.lua") != LUA_OK) {
        std::cerr << "ERROR Lua : " << lua_tostring(Lua_state, -1) << std::endl;
        lua_pop(Lua_state, 1);
    }
}

/**
 * @brief Destructor of the Lua class
 */
Lua::~Lua()
{
    lua_close(Lua_state);
}

/**
 * @brief GetValue the sprite frames from a lua file
 * @param spriteName The name of the sprite to get
 * @return The frames of the sprite
 */
std::vector<Frame> Lua::getSpriteFrames(const std::string& spriteName) {
    std::vector<Frame> frames;

    lua_getglobal(Lua_state, "getSpriteFrame");
    lua_pushstring(Lua_state, spriteName.c_str());

    if (lua_pcall(Lua_state, 1, 1, 0) != LUA_OK) {
        std::cerr << "Erreur Lua: " << lua_tostring(Lua_state, -1) << std::endl;
        lua_pop(Lua_state, 1);
        return frames;
    }

    if (lua_istable(Lua_state, -1)) {
        lua_pushnil(Lua_state);
        while (lua_next(Lua_state, -2)) {
            if (lua_istable(Lua_state, -1)) {
                lua_getfield(Lua_state, -1, "x");
                lua_Integer x = lua_tointeger(Lua_state, -1);
                lua_pop(Lua_state, 1);

                lua_getfield(Lua_state, -1, "y");
                lua_Integer y = lua_tointeger(Lua_state, -1);
                lua_pop(Lua_state, 1);

                lua_getfield(Lua_state, -1, "width");
                lua_Integer width = lua_tointeger(Lua_state, -1);
                lua_pop(Lua_state, 1);

                lua_getfield(Lua_state, -1, "height");
                lua_Integer height = lua_tointeger(Lua_state, -1);
                lua_pop(Lua_state, 1);

                frames.emplace_back(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height));
            }
            lua_pop(Lua_state, 1);
        }
    }

    lua_pop(Lua_state, 1);
    return frames;
}

/**
 * @brief GetValue a string from a lua function
 * @param FunctionName The name of the function to call
 * @param AssetID The ID of the asset to get
 * @return The string found
 */
std::string Lua::getString(const std::string& FunctionName, const std::string& AssetID) {
    std::string path;

    lua_getglobal(Lua_state, FunctionName.c_str());
    lua_pushstring(Lua_state, AssetID.c_str());

    if (lua_pcall(Lua_state, 1, 1, 0) != LUA_OK) {
        std::cerr << "[Error] Lua (" << AssetID << "): " << lua_tostring(Lua_state, -1) << std::endl;
        lua_pop(Lua_state, 1);
        return path;
    }

    if (lua_isstring(Lua_state, -1)) {
        path = lua_tostring(Lua_state, -1);
    } else {
        std::cerr << "[Error] : The string research with " << FunctionName << " for " << AssetID << "was not found." << std::endl;
    }

    lua_pop(Lua_state, 1);
    return path;
}

/**
 * @brief GetValue a float from a lua function
 * @param FunctionName The name of the function to call
 * @param AssetID The ID of the asset to get
 * @return The float found
 */
float Lua::getFloat(const std::string& FunctionName, const std::string& AssetID) {
    float volume = 0.0f;

    lua_getglobal(Lua_state, FunctionName.c_str());
    lua_pushstring(Lua_state, AssetID.c_str());

    if (lua_pcall(Lua_state, 1, 1, 0) != LUA_OK) {
        std::cerr << "[Error] Lua (" << FunctionName << "): " << lua_tostring(Lua_state, -1) << std::endl;
        lua_pop(Lua_state, 1);
        return volume;
    }

    if (lua_isnumber(Lua_state, -1)) {
        volume = static_cast<float>(lua_tonumber(Lua_state, -1));
    } else {
        std::cerr << "[Erreur] : the requested resource is not a float." << std::endl;
    }

    lua_pop(Lua_state, 1);
    return volume;
}

/**
 * @brief GetValue a boolean from a lua function
 * @param FunctionName The name of the function to call
 * @param AssetID The ID of the asset to get
 * @return The boolean found
 */
bool Lua::getBool(const std::string& FunctionName, const std::string& AssetID) {
    bool result = false;

    lua_getglobal(Lua_state, FunctionName.c_str());
    lua_pushstring(Lua_state, AssetID.c_str());

    if (lua_pcall(Lua_state, 1, 1, 0) != LUA_OK) {
        std::cerr << "[Error] Lua (" << FunctionName << "): " << lua_tostring(Lua_state, -1) << std::endl;
        lua_pop(Lua_state, 1);
        return result;
    }

    if (lua_isboolean(Lua_state, -1)) {
        result = lua_toboolean(Lua_state, -1);
    } else {
        std::cerr << "[Erreur] : the requested resource is not a boolean." << std::endl;
    }

    lua_pop(Lua_state, 1);
    return result;
}
