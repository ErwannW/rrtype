//
// Created by louis on 09/01/2025.
//

// ENGINE INCLUDES
#include "../../Engine/Registry.hpp"
#include "../../Engine/Commons/Components/VelocityComponent.hpp"

// LOCAL INCLUDES
#include "GameNetwork.hpp"
#include "../Component/ComponentTekken.hpp"

void RType::RtypeNetwork::ReceivedParalaxOrderFromServer(const NetParalax &gamePacket, MGE::World &world)
{
    std::shared_ptr<MGE::Entity> entity = world.GetRegistry().CreateEntity();
    if (gamePacket.spriteID1 == MGE::HashString("TekkenSpriteStage01"))
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("TekkenSpriteStage01", 100, 100));
    else if (gamePacket.spriteID1 == MGE::HashString("TekkenSpriteStage02"))
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("TekkenSpriteStage02", 100, 100));
    else if (gamePacket.spriteID1 == MGE::HashString("TekkenSpriteStage03"))
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("TekkenSpriteStage03", 100, 100));
    else if (gamePacket.spriteID1 == MGE::HashString("TekkenSpriteStage04"))
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("TekkenSpriteStage04", 100, 100));

    world.GetRegistry().EmplaceComponent<MGE::Common::TransformComponent>(*entity, MGE::Vector3f(50.0f, 50.0f, 0.0f));
    world.GetRegistry().EmplaceComponent<BackgroundComponent>(*entity, BackgroundComponent(gamePacket.speed1));

    std::shared_ptr<MGE::Entity> entity2 = world.GetRegistry().CreateEntity();
    if (gamePacket.spriteID1 == MGE::HashString("TekkenSpriteStage01"))
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity2, SpriteComponent("TekkenSpriteStage01", 100, 100));
    else if (gamePacket.spriteID1 == MGE::HashString("TekkenSpriteStage02"))
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity2, SpriteComponent("TekkenSpriteStage02", 100, 100));
    else if (gamePacket.spriteID1 == MGE::HashString("TekkenSpriteStage03"))
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity2, SpriteComponent("TekkenSpriteStage03", 100, 100));
    else if (gamePacket.spriteID1 == MGE::HashString("TekkenSpriteStage04"))
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity2, SpriteComponent("TekkenSpriteStage04", 100, 100));

    world.GetRegistry().EmplaceComponent<MGE::Common::TransformComponent>(*entity2, MGE::Vector3f(50.0f, 50.0f, 0.0f));
    world.GetRegistry().EmplaceComponent<BackgroundComponent>(*entity2, BackgroundComponent(gamePacket.speed2));
}

void RType::RtypeNetwork::ReceivedRPCEntity(const ManageEntity& gamePacket, MGE::World& world)
{
    std::shared_ptr<MGE::Entity> entity = world.GetRegistry().CreateEntity();
    entity->SetNetId(gamePacket.netId);
    entity->isReplicated = true;
    entity->owner = gamePacket.playerID;

    switch (gamePacket.entityType) {
        case EntityType::Player:
            if (gamePacket.entityID == MGE::HashString("TekkenSpriteFighterRyu"))
                world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("TekkenSpriteFighterRyu", 35, 35));
            else if (gamePacket.entityID == MGE::HashString("TekkenSpriteFighterKen"))
                world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("TekkenSpriteFighterKen", 35, 35));

            world.GetRegistry().EmplaceComponent<SpaceshipComponent>(*entity, SpaceshipComponent(false, 2));
            world.GetRegistry().EmplaceComponent<MGE::Common::TransformComponent>(*entity, gamePacket.entityPosition);
            world.GetRegistry().EmplaceComponent<MGE::Common::VelocityComponent>(*entity);
        case EntityType::Bullet:
            if (gamePacket.entityID == MGE::HashString("player_base_projectile")) {
                world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("player_base_projectile", 4, 4));
                world.GetRegistry().EmplaceComponent<SongComponent>(*entity, SongComponent("laser_type_1", true));
                world.GetRegistry().EmplaceComponent<AutoDestroyComponent>(*entity, AutoDestroyComponent(2));
            }
            world.GetRegistry().EmplaceComponent<MGE::Common::TransformComponent>(*entity, gamePacket.entityPosition);
            break;
        case EntityType::PowerUp:
            break;
        case EntityType::Explosion:
            if (gamePacket.entityID == MGE::HashString("explosion_type_1"))
                world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("explosion_type_1", 100, 100));
            break;
        case EntityType::LevelSond:
            if (gamePacket.entityID == MGE::HashString("TekkenSongStage"))
                world.GetRegistry().EmplaceComponent<SongComponent>(*entity, SongComponent("TekkenSongStage", true));
            break;
        case EntityType::Delete:
            world.GetRegistry().DestroyEntity(world.GetRegistry().GetEntityByNetID(gamePacket.netId));
        case EntityType::UI:
            break;
        case EntityType::Enemy:
            break;
        }
}

void RType::RtypeNetwork::ReceivedServerOrderGameManagement(const ServerOrderGameManagement &gamePacket, MGE::World &world)
{
    world.gameState = gamePacket.gameState;
}
