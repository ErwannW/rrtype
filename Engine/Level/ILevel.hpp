//
// Created by nailperreau on 23/11/24.
//

#ifndef ILEVEL_HPP
#define ILEVEL_HPP

#include <string>
#include "EngineObject.hpp"
#include "Types.hpp"

namespace MGE
{
    class World;

    class ILevel : public IEngineObject
    {
        public:
            /**
             * Callback when the level is started.
             */
            virtual void OnStart() = 0;
            /**
             * Callback when the level is closed.
             */
            virtual void OnClose() = 0;
            /**
             * Callback when a player left.
             */
            virtual void OnPlayerLeft(ClientId playerId) = 0;

            /**
             * Called each update.
             */
            virtual void OnUpdate() = 0;
    };
} // MGE

#endif //ILEVEL_HPP
