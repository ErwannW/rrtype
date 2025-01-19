//
// Created by louis on 09/01/2025.
//

#include "SystemeComponent.hpp"

void ComponentRType::ProjectileSystemServer(MGE::World &world, MGE::EntityRef& entity, ProjectileComponent &projectileComponent, MGE::Common::TransformComponent &transformComponent) {
    auto& registry = world.GetRegistry();
    auto& projectile_entity = entity.entity;
    bool destroyProjectile = false;

    if (projectileComponent.orientation == 0) {
        for (std::size_t i = 0; i < registry.GetComponents<EnnemyComponent>().size(); ++i) {
            auto & ennemy = registry.GetComponents<EnnemyComponent>()[i];
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


                        RType::ManageEntity createExplosionPacket(transformEnemy.position, RType::EntityType::Explosion,
                          MGE::HashString("explosion_type_1"), explosion->GetNetId(), explosion->owner);
                        RType::RtypeNetwork::SendRPCEntity(createExplosionPacket, world);

                        for (auto &player : world.GetPlayers()) {
                            if (player.playerId == projectile_entity->owner) {
                                player.score += 20;
                            }
                        }
                        RType::RtypeNetwork::SendRPCLifeUpdate(world);
                    }
                    transformComponent.position.x = 200.f;
                    destroyProjectile = true;
                }
            }
        }
    }

    if ((projectileComponent.orientation == 0 && transformComponent.position.x > 105.0f)
        || (projectileComponent.orientation == 1 && transformComponent.position.x < -5.0f) || destroyProjectile == true) {
        if (static_cast<bool>(MGE::BuildTarget::BuildType::Server)) {
            return registry.DestroyEntity(projectile_entity);
        }

    }
}

ProjectileComponent::ProjectileComponent(int orientation, int type, int id)
    : orientation(orientation), type(type), id(id) {}

void ComponentRType::SendDegat(float positionX, float positionY) {

}

void ProjectileComponent::SetupReflection()
{
    Class.RegisterProperty(orientation);
    Class.RegisterProperty(type);
    Class.RegisterProperty(id);
}
