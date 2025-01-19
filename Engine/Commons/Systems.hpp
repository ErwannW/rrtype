//
// Created by np on 11/27/24.
//

#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include "World.hpp"
#include "Components/Collider.hpp"
#include "Components/Transform.hpp"
#include "Components/RigidBody.hpp"
#include "Components/VelocityComponent.hpp"

namespace MGE::Common
{
    void VelocitySystemFunc(World& world, TransformComponent& transform, VelocityComponent& velocity);
    void PhysicsSystemFunc(World& world, TransformComponent &transform, VelocityComponent &velocity, RigidBodyComponent &rigidBody);
    void NewSpriteSystemFunc(World &world, SpriteComponent &spriteComponent, TransformComponent& transform);

    /**
     * Moves a transform accordingly to the (scaled) velocity of a velocity component.
     */
    inline auto VelocitySystem = std::make_shared<System>("Velocity", VelocitySystemFunc, System::ClientAndServer);

    /**
     * Updates a velocity component according to gravity.
     */
    inline auto PhysicsSystem  = std::make_shared<System>("Physics", PhysicsSystemFunc, System::ClientAndServer);

    /**
     * Displays a sprite at the position, rotation and scale given by the transform component.
     */
    inline auto SpriteSystem   = std::make_shared<System>("Sprite", NewSpriteSystemFunc, System::ClientOnly);
} // Common
// MGE

#endif //SYSTEMS_HPP
