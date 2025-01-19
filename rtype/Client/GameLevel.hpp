//
// Created by louiscroci on 26/11/24.
//

#ifndef GAMELEVEL_HPP
#define GAMELEVEL_HPP

#include "Entity.hpp"
#include "Level/Level.hpp"
#include "GeneriqueEntity/GeneriqueComponent.hpp"
#include "../component/SystemeComponent.hpp"

namespace MGE
{
    class GameLevel : public Level
    {
        public:
            explicit GameLevel(World& world);
            void OnStart() override;
            void OnClose() override;
            void OnUpdate() override;

            [[nodiscard]] std::string_view GetName() const override;

        private:
            std::shared_ptr<MGE::Entity> LevelBackground;
            std::shared_ptr<MGE::Entity> life1;
            std::shared_ptr<MGE::Entity> life2;
            std::shared_ptr<MGE::Entity> life3;

            ComponentRType componentRType;
            int life = 3;
    };
} // MGE

#endif //GAMELEVEL_HPP
