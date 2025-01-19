/*
** EPITECH PROJECT, 2024
** Engine
** File description:
** LevelOne
*/

#include <chrono>

// ENGINE INCLUDES
#include "../../Engine/World.hpp"

// LOCAL INCLUDES
#include "ServerGame.hpp"
#include "ServerLobby.hpp"
#include "../Component/ComponentTekken.hpp"

namespace MGE::Server
{
    void ServerGame::SendBackgroundToClient() const
    {
        RType::NetParalax paralax;
        paralax.spriteID1 = HashString(paralaxConfig[0].spriteId);
        paralax.spriteID2 = HashString(paralaxConfig[0].spriteId);
        paralax.speed1 = paralaxConfig[0].speed;
        paralax.speed2 = paralaxConfig[0].speed;
        network.SendRPCParalax(paralax, world);
    }
}
