//
// Created by louis on 09/01/2025.
//

#include "ComponentTekken.hpp"

void ComponentRType::ProjectileSystemServer(MGE::World &world, MGE::EntityRef& entity, ProjectileComponent &projectileComponent, MGE::Common::TransformComponent &transformComponent)
{
    auto &registry = world.GetRegistry();
    const auto projectile_entity = entity.entity;

    if (projectileComponent.orientation == 0) {
        auto &enemyComponents = registry.GetComponents<EnnemyComponent>();
        for (auto & ennemy : enemyComponents) {
            if (!ennemy.has_value())
                continue;

            auto &enemyComponent = ennemy.value();
            auto enemyEntity = registry.GetEntityByComponent(enemyComponent);
            if (!enemyEntity->IsEnabled())
              continue;
            auto &transformEnemy = registry.GetComponent<MGE::Common::TransformComponent>(*enemyEntity);

            if (enemyComponent.config.heatBoxType == MGE::Server::HeatBoxType::Circle) {
                if (CollisionCircle(transformComponent.position, transformEnemy.position ,static_cast<float>(enemyComponent.config.heatBoxSize))) {
                    enemyComponent.hp -= 20;
                    if (enemyComponent.hp <= 0) {
                        registry.DestroyEntity(enemyEntity);

                        std::shared_ptr<MGE::Entity> explosion = world.GetRegistry().CreateEntity();
                        explosion->SetNetId(world.GetNewUniqueNetID());
                        explosion->isReplicated = true;
                        explosion->owner = entity.entity->owner;

                        world.GetRegistry().EmplaceComponent<AutoDestroyComponent>(explosion, AutoDestroyComponent(3));

                        RType::ManageEntity createExplosionPacket(transformEnemy.position, RType::EntityType::Explosion,
                          MGE::HashString("explosion_type_1"), explosion->GetNetId(), explosion->owner);
                        RType::RtypeNetwork::SendRPCEntity(createExplosionPacket, world);

                        for (auto &player : world.GetPlayers()) {
                            if (player.playerId == projectile_entity->owner)
                                player.score += 20;
                        }
                    }

                    return registry.DestroyEntity(projectile_entity);
                }
            }
        }
    }

    if ((projectileComponent.orientation == 0 && transformComponent.position.x > 105.0f)
        || (projectileComponent.orientation == 1 && transformComponent.position.x < -5.0f))
            if (projectile_entity == nullptr)
                return;
}

ProjectileComponent::ProjectileComponent(int orientation, int type, int id)
    : orientation(orientation), type(type), id(id) {}

void ComponentRType::SendDegat(float positionX, float positionY) {}

void ProjectileComponent::SetupReflection()
{
    Class.RegisterProperty(orientation);
    Class.RegisterProperty(type);
    Class.RegisterProperty(id);
}
