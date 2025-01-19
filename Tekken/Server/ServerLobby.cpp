//
// Created by nailperreau on 25/11/24.
//

// ENGINE INCLUDES
#include "../../Engine/World.hpp"
#include "../../Engine/Player.hpp"

// LOCAL INCLUDES
#include "ServerLobby.hpp"

namespace MGE
{
    namespace Server
    {
        ServerLobby::ServerLobby(World &world) : Level(world) {}

        void ServerLobby::OnStart()
        {
            Level::OnStart();
        }

        void ServerLobby::OnUpdate()
        {
            int nbPlayerReady = 0;

            Level::OnUpdate();
            const std::vector<Player>& players = world.GetPlayers();
            for (const auto& player: players)
                if (player.gameEvent == RType::EventType::InGame)
                    nbPlayerReady ++;
            if (nbPlayerReady == numPlayersToStartGame)
                world.SetLevel(std::make_shared<ServerGame>(world));
        }

        std::string_view ServerLobby::GetName() const
        {
            return "Server Lobby";
        }
    }
} // MGE