//
// Created by nailperreau on 25/11/24.
//

#include "ServerLobby.hpp"

#include "../../Engine/World.hpp"
#include "Player.hpp"


namespace MGE
{
    namespace Server
    {
        ServerLobby::ServerLobby(World &world) : Level(world)
        {
        }

        void ServerLobby::OnStart()
        {
            Level::OnStart();
        }

        void ServerLobby::OnUpdate()
        {
            int nbPlayerReady = 0;

            Level::OnUpdate();
            std::vector<Player> &players = world.GetPlayers();
            for (int i = 0; i < players.size(); i++)
              if (players[i].gameEvent == RType::EventType::InGame)
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