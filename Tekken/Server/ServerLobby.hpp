//
// Created by louiscroci on 26/11/24.
//

#ifndef ServerLobby_HPP
#define ServerLobby_HPP

// ENGINE
#include "../../Engine/Level/Level.hpp"

// LOCAL INCLUDES
#include "ServerGame.hpp"
#include "../Network/GameNetwork.hpp"

namespace MGE::Server
{
    class ServerLobby: public Level
    {
        public:
            explicit ServerLobby(World& world);
            void OnStart() override;
            void OnUpdate() override;

            [[nodiscard]] std::string_view GetName() const override;

        private:
            uint8 numPlayersToStartGame = 1;
            RType::RtypeNetwork Network;
    };
} // MGE

#endif //ServerLobby_HPP
