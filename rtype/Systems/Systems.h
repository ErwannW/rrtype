//
// Created by np on 12/11/24.
//

#ifndef SYSTEMS_H
#define SYSTEMS_H
#include <memory>
#include "../component/SystemeComponent.hpp"

#include "System/System.hpp"

namespace RType
{
    inline std::shared_ptr<MGE::System> SpaceshipSystem = std::make_shared<MGE::System>("Spaceship", ComponentRType::SpaceShipSystem);
    inline std::shared_ptr<MGE::System> ProjectileSystemServer = std::make_shared<MGE::System>("ProjectileSystemServer", ComponentRType::ProjectileSystemServer);
    inline std::shared_ptr<MGE::System> BackgroundSystem = std::make_shared<MGE::System>("Background", ComponentRType::BackgroundSystem);
    inline std::shared_ptr<MGE::System> EnnemySystem = std::make_shared<MGE::System>("EnnemySystem", ComponentRType::EnnemySystem);
    inline std::shared_ptr<MGE::System> LifeSysteme = std::make_shared<MGE::System>( "LifeSysteme", ComponentRType::LifeSystem);
    inline std::shared_ptr<MGE::System> ScoreSysteme = std::make_shared<MGE::System>( "ScoreSysteme", ComponentRType::ScoreSysteme);
    inline std::shared_ptr<MGE::System> ProjectileAnimationsSystemServer = std::make_shared<MGE::System>( "ProjectileAnimationsSystemServer", ComponentRType::ProjectileAnimationsSystemServer);
    inline std::shared_ptr<MGE::System> AutoDestroySpriteSysteme = std::make_shared<MGE::System>( "AutoDestroySpriteSysteme", ComponentRType::AutoDestroySpriteSysteme);
    inline std::shared_ptr<MGE::System> AutoDestroySongSysteme = std::make_shared<MGE::System>( "AutoDestroySongSysteme", ComponentRType::AutoDestroySongSysteme);
    inline std::shared_ptr<MGE::System> EnnemyShooterSysteme = std::make_shared<MGE::System>( "EnnemyShooterSysteme", ComponentRType::EnnemyShooterSysteme);

} // RType

#endif //SYSTEMS_H
