//
// Created by np on 11/27/24.
//

#include "Systems.hpp"

#include <ranges>

#include "World.hpp"
#include "Level/Level.hpp"
#include <unordered_set>

namespace MGE::Common
{
    void VelocitySystemFunc(World& world, TransformComponent& transform, VelocityComponent& velocity)
    {
        transform.position += velocity.velocity * world.GetFDeltaSeconds();
    }

    void PhysicsSystemFunc(World &world, TransformComponent &transform, VelocityComponent &velocity, RigidBodyComponent &rigidBody)
    {
        static constexpr double GravityConstant = 6.6743 * 1e-11;

        if (rigidBody.settings.gravitySource == GravitySource::None)
            return;

        if (rigidBody.settings.gravityMass == GravityForce::None)
            return;

        Vector3f gravityDirection;

        if (rigidBody.settings.gravitySource == GravitySource::Point)
            gravityDirection = rigidBody.settings.gravityPoint - transform.position;
        else
            gravityDirection = rigidBody.settings.gravityDirection;

        const Vector3f normalizedDirection = gravityDirection.Normalized();
        const Vector3f force = (normalizedDirection * rigidBody.settings.gravityMass * rigidBody.settings.mass * static_cast<float>(GravityConstant)) / (gravityDirection * gravityDirection);

        velocity.velocity += (force / rigidBody.settings.mass) * world.GetFDeltaSeconds();
    }

    /*
    void ScrollSystem(World &world, ScrollComponent &scroll, TransformComponent &transform, SpriteComponent &sprite)
    {
        const float screenWidth = 200.0;
        const float spriteWidth = 190.0;
        
        transform.position.x += scroll.scroll.x * scroll.scrollSpeed;
        sprite.position_x = transform.position.x;

        if (transform.position.x + spriteWidth <= 100.0) {
            std::shared_ptr<MGE::Entity> current_entity;
            const auto entities = world.GetRegistry().GetEntities();

            for (auto &entity : entities) {
                if (world.GetRegistry().HasComponent<ScrollComponent, TransformComponent, SpriteComponent>(*entity)) {
                    auto entity_scroll = world.GetRegistry().GetComponent<ScrollComponent>(*entity);
                    if (&entity_scroll == &scroll) {
                        world.GetRegistry().DestroyEntity(entity);
                        return;
                    }
                }
            }
        }

        if (transform.position.x + spriteWidth < screenWidth) {
            if (!scroll.hasDuplicated) {
                auto transform_dup = transform;
                auto scroll_dup = scroll;
                auto sprite_dup = sprite;

                transform_dup.position.x += spriteWidth;
                scroll_dup.id += 1;

                scroll.hasDuplicated = true;

                auto duplicate = world.GetRegistry().CreateEntity();
                world.GetRegistry().EmplaceComponent<MGE::TransformComponent>(duplicate, TransformComponent(transform_dup));
                world.GetRegistry().EmplaceComponent<MGE::ScrollComponent>(duplicate, ScrollComponent(scroll_dup));
                world.GetRegistry().EmplaceComponent<SpriteComponent>(duplicate, SpriteComponent(sprite_dup));
            }
        }
    }
    */
   
    void NewSpriteSystemFunc(MGE::World &world, SpriteComponent &spriteComponent, TransformComponent &transform)
    {
        world.engine.GetRenderer<SfmlDisplay>()->DrawSprite(spriteComponent.texture_id,
            static_cast<int>(transform.position.x), static_cast<int>(transform.position.y),
            static_cast<int>(spriteComponent.width * transform.scale.x),
            static_cast<int>(spriteComponent.height * transform.scale.y));
        //world.engine.GetRenderer<SfmlDisplay>()->UpdateElementPosition(spriteComponent, {transform.position, transform.scale, transform.rotation});
        //world.engine.GetRenderer<SfmlDisplay>()->UpdateElement(spriteComponent);
    }
} // Common
// MGE