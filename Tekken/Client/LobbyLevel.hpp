//
// Created by louiscroci on 26/11/24.
//

#ifndef LOBBYLEVEL_HPP
#define LOBBYLEVEL_HPP

// ENGINE INCLUDES
#include "../../Engine/Entity.hpp"
#include "../../Engine/Level/Level.hpp"
#include "../../Engine/GeneriqueEntity/GeneriqueComponent.hpp"

// LOCAL INCLUDES
#include "GameLevel.hpp"
#include "../Network/GameNetwork.hpp"

namespace MGE
{
    class LobbyLevel : public Level
    {
        public:
            explicit LobbyLevel(World& world);
            void OnStart() override;
            void OnClose() override;
            void OnUpdate() override;

            [[nodiscard]] std::string_view GetName() const override;

        private:
            std::shared_ptr<MGE::Entity> LobbyPage;
            std::shared_ptr<MGE::Entity> Song;
            std::shared_ptr<MGE::Entity> ButtonSetting;
            std::shared_ptr<MGE::Entity> ButtonServer;
            std::shared_ptr<MGE::Entity> ButtonRoom;
            std::shared_ptr<MGE::Entity> ButtonStart;
            std::shared_ptr<MGE::Entity> ExitButton;
            GenericComponent generiqueComponent;
            uint32 StartingLevelTime = 0;
            RType::RtypeNetwork network;
    };
} // MGE

#endif //LOBBYLEVEL_HPP
