//
// Created by nailperreau on 25/11/24.
//

#ifndef MAINMENU_HPP
#define MAINMENU_HPP

// ENGINE INCLUDES
#include "../../Engine/Entity.hpp"
#include "../../Engine/Level/Level.hpp"

// LOCAL INCLUDES
#include "GameLevel.hpp"

namespace MGE
{
    class LoadingLevel : public Level
    {
        public:
            explicit LoadingLevel(World& world);
            void OnStart() override;
            void OnUpdate() override;
            void OnClose() override;

            [[nodiscard]] std::string_view GetName() const override;

        private:
            std::shared_ptr<MGE::Entity> LoadingPage;
            std::shared_ptr<MGE::Entity> Background;
            std::shared_ptr<MGE::Entity> Song;
            std::shared_ptr<MGE::Entity> LoadingAnimation;
            std::shared_ptr<MGE::Entity> LoadingText;
            int _loadingTime = 0;
    };
} // MGE

#endif //MAINMENU_HPP
