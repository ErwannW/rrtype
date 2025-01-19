//
// Created by np on 12/11/24.
//

#ifndef SYSTEMS_H
#define SYSTEMS_H

#include <memory>

// ENGINE INCLUDES
#include "../../Engine/System/System.hpp"

// LOCAL INCLUDES
#include "../Component/ComponentTekken.hpp"

namespace Tekken
{
    inline std::shared_ptr<MGE::System> SpaceshipSystem = std::make_shared<MGE::System>("Spaceship", ComponentRType::SpaceShipSystem);
    inline std::shared_ptr<MGE::System> ProjectileSystemServer = std::make_shared<MGE::System>("ProjectileSystemServer", ComponentRType::ProjectileSystemServer);
    inline std::shared_ptr<MGE::System> BackgroundSystem = std::make_shared<MGE::System>("Background", ComponentRType::BackgroundSystem);
    inline std::shared_ptr<MGE::System> EnnemySystem = std::make_shared<MGE::System>("EnnemySystem", ComponentRType::EnnemySystem);
    inline std::shared_ptr<MGE::System> AutoDestroySysteme = std::make_shared<MGE::System>( "AutoDestroySysteme", ComponentRType::AutoDestroySysteme);
} // RType

#endif //SYSTEMS_H
