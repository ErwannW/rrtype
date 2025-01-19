/*
** EPITECH PROJECT, 2024
** Engine
** File description:
** LevelOne
*/

#include <chrono>
#include "../component/SystemeComponent.hpp"
#include "ServerGame.hpp"
#include "../../Engine/World.hpp"
#include "ServerLobby.hpp"

namespace MGE::Server
{
      void ServerGame::SendBackgroundToClient() const
      {
          RType::NetParalax paralax;
          paralax.spriteID1 = HashString(paralaxConfig[0].spriteId);
          paralax.spriteID2 = HashString(paralaxConfig[1].spriteId);
          paralax.speed1 = paralaxConfig[0].speed;
          paralax.speed2 = paralaxConfig[1].speed;
          network.SendRPCParalax(paralax, world);
      }
}