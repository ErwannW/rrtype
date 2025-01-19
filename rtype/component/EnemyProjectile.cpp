//
// Created by louis on 18/01/25.
//

#include "SystemeComponent.hpp"
#include "../Network/GameNetwork.hpp"

bool generateRandomBool() {
    int randomNumber = rand() % 3 + 1;
    std::cout << randomNumber << std::endl;
    return randomNumber == 1;
}

void ComponentRType::EnnemyShooterSysteme(MGE::World &world, EnnemyShooterComponent &ennemyShooterComponent, MGE::Common::TransformComponent &transformComponent)
{
    if (ennemyShooterComponent.ProjectileType == "projectile_bose_2_creation") {
        ennemyShooterComponent.lastShot += world.GetDeltaSeconds();
        if (ennemyShooterComponent.EnemyType == "boss_type_2" && ennemyShooterComponent.lastShot > 0.5f && generateRandomBool()) {
            ennemyShooterComponent.lastShot = 0;
            std::shared_ptr<MGE::Entity> enemy = world.GetRegistry().CreateEntity();
            enemy->isReplicated = true;
            enemy->owner = 0;
            enemy->SetNetId(world.GetNewUniqueNetID());

            int posX = transformComponent.position.x - 10;
            int posY = transformComponent.position.y + (rand() % 2 == 0 ? -20 : 20);

            world.GetRegistry().EmplaceComponent<MGE::Common::TransformComponent>(enemy, MGE::Common::TransformComponent(MGE::Vector3f(static_cast<float>(posX), static_cast<float>(posY), 0.f)));
            world.GetRegistry().EmplaceComponent<EnnemyComponent>(enemy, EnnemyComponent(ennemyShooterComponent.config.speed, ennemyShooterComponent.config.Point_of_live, ennemyShooterComponent.config.isBosse, ennemyShooterComponent.config.movingUp, ennemyShooterComponent.config, "linearEnemy"));

            RType::ManageEntity createPlayerEntityPacket(MGE::Vector3f(static_cast<float>(posX), static_cast<float>(posY), 0.f),
                RType::EntityType::Enemy, MGE::HashString(ennemyShooterComponent.ProjectileType), enemy->GetNetId(), 0);
            std::future<void> result = std::async(std::launch::async, [&]() {
                RType::RtypeNetwork::SendRPCEntity(createPlayerEntityPacket, world);
            });
            result.get();
        }
    }
}

EnnemyShooterComponent::EnnemyShooterComponent(std::string EnemyType, std::string ProjectileType, MGE::Server::EnemyConfig &config) : EnemyType(EnemyType), ProjectileType(ProjectileType), config(config) {}

void EnnemyShooterComponent::SetupReflection()
{
}


