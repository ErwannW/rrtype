//
// Created by louiscroci on 26/11/24.
//

#ifndef GAMELEVEL_HPP
#define GAMELEVEL_HPP

// ENGINE INCLUDES
#include "../../Engine/Entity.hpp"
#include "../../Engine/Level/Level.hpp"

// LOCAL INCLUDES
#include "../Component/ComponentTekken.hpp"

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
            std::shared_ptr<MGE::Entity> Song;
            ComponentRType cRtype;
    };
} // MGE

#endif //GAMELEVEL_HPP
