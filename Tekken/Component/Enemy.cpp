//
// Created by louis on 09/01/2025.
//

#include "ComponentTekken.hpp"

void ComponentRType::EnnemySystem(MGE::World &world, MGE::EntityRef& entity, EnnemyComponent &ennemyComponent, MGE::Common::TransformComponent &transformComponent)
{
    if (ennemyComponent.movement == "sinusoidal") {
        if (ennemyComponent.movingUp) {
            transformComponent.position.y -= ennemyComponent.speed * world.GetFDeltaSeconds() * 2.0f;
            if (transformComponent.position.y <= 0.0f)
                ennemyComponent.movingUp = false;
        } else {
            transformComponent.position.y += ennemyComponent.speed * world.GetFDeltaSeconds() * 2.0f;
            if (transformComponent.position.y >= 100.0f)
                ennemyComponent.movingUp = true;
        }
    }

    auto &spaceshipComponents = world.GetRegistry().GetComponents<SpaceshipComponent>();

    for (auto & spaceshipComponent : spaceshipComponents) {
        if (!spaceshipComponent.has_value())
            continue;

        auto &spaceshipeComponent = spaceshipComponent.value();
        auto spaceshipEntity = world.GetRegistry().GetEntityByComponent(spaceshipeComponent);
        auto &transformSpaceship = world.GetRegistry().GetComponent<MGE::Common::TransformComponent>(*spaceshipEntity);

        if (CollisionCircle(transformComponent.position, transformSpaceship.position, static_cast<float>(ennemyComponent.config.heatBoxSize))) {
            for (auto &player : world.GetPlayers()) {
                if (player.playerId == spaceshipEntity->owner) {
                    world.GetRegistry().DestroyEntity(entity.entity);
                    SendDegat(transformComponent.position.x, transformComponent.position.y);
                    player.life -= 1;
                    if (player.life <= 0)
                        return world.GetRegistry().DestroyEntity(spaceshipEntity);
                    return;
                }
            }
        }
    }
    if (ennemyComponent.hp <= 0 && transformComponent.position.x < -5.0f)
        return world.GetRegistry().DestroyEntity(entity.entity);
    if (ennemyComponent.isBoss && transformComponent.position.x <= 87.0f)
        return;
    transformComponent.position.x -= ennemyComponent.speed * world.GetFDeltaSeconds();
}

EnnemyComponent::EnnemyComponent(float speed, int hp, bool isBoss, bool movingUp, MGE::Server::EnemyConfig &config, std::string movement)
    : speed(speed), hp(hp), isBoss(isBoss), movingUp(true), config(config), movement(std::move(movement)) {}

void EnnemyComponent::SetupReflection()
{
    Class.RegisterProperty(speed);
    Class.RegisterProperty(hp);
    Class.RegisterProperty(isBoss);
    Class.RegisterProperty(movingUp);
    Class.RegisterProperty(movement);
}
