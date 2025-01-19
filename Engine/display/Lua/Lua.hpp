/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Lua
*/

#ifndef LUA_HPP_
#define LUA_HPP_

#include <iostream>
#include <vector>


struct Frame {
    int x, y, width, height;

    Frame(int x, int y, int width, int height)
        : x(x), y(y), width(width), height(height) {}
};


class Lua {
    public:
        Lua();
        ~Lua();
        std::vector<Frame> getSpriteFrames(const std::string& spriteName);
        std::string getString(const std::string& FunctionName, const std::string& AssetID);
        float getFloat(const std::string& FunctionName, const std::string& AssetID);
        bool getBool(const std::string& FunctionName, const std::string& AssetID);

    private:
        struct lua_State* Lua_state;

};

#endif /* !LUA_HPP_ */
