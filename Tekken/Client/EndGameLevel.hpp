//
// Created by louiscroci on 26/11/24.
//

#ifndef ENDGAMELEVEL_HPP
#define ENDGAMELEVEL_HPP

#include "../../Engine/Entity.hpp"
#include "../../Engine/Level/Level.hpp"
#include "../../Engine/GeneriqueEntity/GeneriqueComponent.hpp"

// LOCAL INCLUDES
#include "../Network/GameNetwork.hpp"

namespace MGE
{
    class EndGameLevel : public Level
    {
        public:
            explicit EndGameLevel(World& world);
            void OnStart() override;
            void OnClose() override;

            [[nodiscard]] std::string_view GetName() const override;

        private:
            std::shared_ptr<Entity> BackgroundImage;
            std::shared_ptr<Entity> Background;
            std::shared_ptr<Entity> Song;

            std::shared_ptr<Entity> NextStageButton;
            std::shared_ptr<Entity> LobbyButton;
            std::shared_ptr<Entity> EndGameText;
            std::shared_ptr<Entity> YourScoreText;
            std::shared_ptr<Entity> TeamScoreText;
            std::shared_ptr<Entity> CreditButton;

            GenericComponent generiqueComponent;
            RType::RtypeNetwork network;
    };
} // MGE

#endif //ENDGAMELEVEL_HPP
