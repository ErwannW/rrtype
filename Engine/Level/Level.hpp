//
// Created by nailperreau on 25/11/24.
//

#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "ILevel.hpp"

namespace MGE
{
    class Level : public ILevel
    {
        public:
            explicit Level(World& world);

            void OnStart() override;
            void OnClose() override;

            void OnPlayerLeft(ClientId playerId) override;

            void OnUpdate() override;

        protected:
            World& world;
    };
} // MGE

#endif //LEVEL_HPP
