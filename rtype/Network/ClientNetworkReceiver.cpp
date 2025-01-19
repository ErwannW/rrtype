//
// Created by louis on 09/01/2025.
//

#include "GameNetwork.hpp"
#include "Registry.hpp"
#include "Commons/Components/VelocityComponent.hpp"
#include "../component/SystemeComponent.hpp"

void RType::RtypeNetwork::ReceivedParalaxOrderFromServer(const NetParalax &gamePacket, MGE::World &world) {

    std::shared_ptr<MGE::Entity> entity = world.GetRegistry().CreateEntity();
    std::shared_ptr<MGE::Entity> entity3 = world.GetRegistry().CreateEntity();
    if (MGE::HashString("paralax_stage01_1") == gamePacket.spriteID1) {
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("paralax_stage01_1", 200, 105));
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity3, SpriteComponent("paralax_stage01_1", 200, 105));
    }
    if (MGE::HashString("paralax_stage01_2") == gamePacket.spriteID1) {
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("paralax_stage01_2", 200, 105));
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity3, SpriteComponent("paralax_stage01_2", 200, 105));
    }
    if (MGE::HashString("paralax_stage02_2") == gamePacket.spriteID1) {
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("paralax_stage02_2", 200, 105));
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity3, SpriteComponent("paralax_stage02_2", 200, 105));
    }
    if (MGE::HashString("paralax_stage02_1") == gamePacket.spriteID1) {
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("paralax_stage02_1", 200, 105));
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity3, SpriteComponent("paralax_stage02_1", 200, 105));
    }
    if (MGE::HashString("paralax_stage03_2") == gamePacket.spriteID1) {
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("paralax_stage03_2", 200, 105));
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity3, SpriteComponent("paralax_stage03_2", 200, 105));
    }
    if (MGE::HashString("paralax_stage04_2") == gamePacket.spriteID1) {
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("paralax_stage04_2", 200, 105));
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity3, SpriteComponent("paralax_stage04_2", 200, 105));
    }
    world.GetRegistry().EmplaceComponent<MGE::Common::TransformComponent>(*entity, MGE::Vector3f(0.0f, 50.0f, 0.0f));
    world.GetRegistry().EmplaceComponent<BackgroundComponent>(*entity, BackgroundComponent(gamePacket.speed1));
    world.GetRegistry().EmplaceComponent<MGE::Common::TransformComponent>(*entity3, MGE::Vector3f(200.0f, 50.0f, 0.0f));
    world.GetRegistry().EmplaceComponent<BackgroundComponent>(*entity3, BackgroundComponent(gamePacket.speed1));

    std::shared_ptr<MGE::Entity> entity2 = world.GetRegistry().CreateEntity();
    std::shared_ptr<MGE::Entity> entity4 = world.GetRegistry().CreateEntity();

    if (MGE::HashString("paralax_stage01_1") == gamePacket.spriteID2) {
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity2, SpriteComponent("paralax_stage01_1", 200, 105));
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity4, SpriteComponent("paralax_stage01_1", 200, 105));
    }
    if (MGE::HashString("paralax_stage01_2") == gamePacket.spriteID2) {
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity2, SpriteComponent("paralax_stage01_2", 200, 105));
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity4, SpriteComponent("paralax_stage01_2", 200, 105));
    }
    if (MGE::HashString("paralax_stage02_2") == gamePacket.spriteID2) {
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity2, SpriteComponent("paralax_stage02_2", 200, 105));
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity4, SpriteComponent("paralax_stage02_2", 200, 105));
    }
    if (MGE::HashString("paralax_stage02_1") == gamePacket.spriteID2) {
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity2, SpriteComponent("paralax_stage02_1", 200, 105));
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity4, SpriteComponent("paralax_stage02_1", 200, 105));
    }
    if (MGE::HashString("paralax_stage03_2") == gamePacket.spriteID2) {
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity2, SpriteComponent("paralax_stage03_2", 200, 105));
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity4, SpriteComponent("paralax_stage03_2", 200, 105));
    }
    if (MGE::HashString("paralax_stage04_2") == gamePacket.spriteID2) {
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity2, SpriteComponent("paralax_stage04_2", 200, 105));
        world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity4, SpriteComponent("paralax_stage04_2", 200, 105));
    }
    world.GetRegistry().EmplaceComponent<MGE::Common::TransformComponent>(*entity2, MGE::Vector3f(0.0f, 50.0f, 0.0f));
    world.GetRegistry().EmplaceComponent<BackgroundComponent>(*entity2, BackgroundComponent(gamePacket.speed2));
    world.GetRegistry().EmplaceComponent<MGE::Common::TransformComponent>(*entity4, MGE::Vector3f(200.0f, 50.0f, 0.0f));
    world.GetRegistry().EmplaceComponent<BackgroundComponent>(*entity4, BackgroundComponent(gamePacket.speed2));
}

void RType::RtypeNetwork::ReceivedRPCEntity(const ManageEntity& gamePacket, MGE::World& world)
{
    if (world.gameState != GameState::InGame)
        return;
    std::shared_ptr<MGE::Entity> entity = world.GetRegistry().CreateEntity();
    entity->SetNetId(gamePacket.netId);
    entity->isReplicated = true;
    entity->owner = gamePacket.playerID;

    switch (gamePacket.entityType) {
        case EntityType::Player:
            if (gamePacket.entityID == MGE::HashString("spaceship_blue")) {
                world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("spaceship_blue", 10, 10));
            } else if (gamePacket.entityID == MGE::HashString("spaceship_green"))
                world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("spaceship_green", 10, 10));
            else if (gamePacket.entityID == MGE::HashString("spaceship_red"))
                world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("spaceship_red", 10, 10));
            else if (gamePacket.entityID == MGE::HashString("spaceship_pink"))
                world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("spaceship_pink", 10, 10));
            world.GetRegistry().EmplaceComponent<SpaceshipComponent>(*entity, SpaceshipComponent(false, 2));
            world.GetRegistry().EmplaceComponent<MGE::Common::TransformComponent>(*entity, gamePacket.entityPosition);
            world.GetRegistry().EmplaceComponent<MGE::Common::VelocityComponent>(*entity);
        case EntityType::Enemy:
            if (gamePacket.entityID == MGE::HashString("enemy_type_1")) {
                world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("enemy_type_1", 10, 10));
            } else if (gamePacket.entityID == MGE::HashString("bose_type_1")) {
                world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("Boss_type_1", 50, 50));
            } else if (gamePacket.entityID == MGE::HashString("enemy_type_2")) {
                world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("enemy_type_2", 20, 20));
            } else if  (gamePacket.entityID == MGE::HashString("enemy_type_3")) {
                world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("enemy_type_3", 5, 5));
            } else if (gamePacket.entityID == MGE::HashString("boss_type_2")) {
                world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("Boss_type_2", 50, 50));
            } else if (gamePacket.entityID == MGE::HashString("projectile_bose_2_creation")) {
                world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("projectile_bose_2_creation", 10, 10));
                world.GetRegistry().EmplaceComponent<ProjectileAnimationsComponent>(*entity, ProjectileAnimationsComponent(1.8f, "projectile_bose_2"));
            } else {
                world.engine.GetLogger<>()->Error("Unknown entity type");
                return;
            }
            world.GetRegistry().EmplaceComponent<MGE::Common::TransformComponent>(*entity, gamePacket.entityPosition);
            break;
        case EntityType::Bullet:
            if (gamePacket.entityID == MGE::HashString("player_base_projectile")) {
                world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("player_base_projectile", 4, 4));
                world.GetRegistry().EmplaceComponent<SongComponent>(*entity, SongComponent("laser_type_1", true));
                world.GetRegistry().EmplaceComponent<AutoDestroyComponent>(*entity, AutoDestroyComponent(1, false, true));
            }
            world.GetRegistry().EmplaceComponent<MGE::Common::TransformComponent>(*entity, gamePacket.entityPosition);
            break;
        case EntityType::PowerUp:
            break;
        case EntityType::Explosion:
            entity->isReplicated = false;
            if (gamePacket.entityID == MGE::HashString("explosion_type_1")) {
                world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("explosion_type_1", 20, 20));
                world.GetRegistry().EmplaceComponent<SongComponent>(*entity, SongComponent("explosion_1", true));
            }
            world.GetRegistry().EmplaceComponent<MGE::Common::TransformComponent>(*entity, MGE::Common::TransformComponent(gamePacket.entityPosition));
            world.GetRegistry().EmplaceComponent<AutoDestroyComponent>(*entity, AutoDestroyComponent(0.7f, true, true));
            break;
        case EntityType::LevelSond:
            entity->isReplicated = false;
            if (gamePacket.entityID == MGE::HashString("Level1Sound")) {
              world.GetRegistry().EmplaceComponent<SongComponent>(*entity, SongComponent("Level1Sound", true));
            }
            if (gamePacket.entityID == MGE::HashString("Level2Sound")) {
              world.GetRegistry().EmplaceComponent<SongComponent>(*entity, SongComponent("Level2Sound", true));
            }
            if (gamePacket.entityID == MGE::HashString("Level3Sound")) {
                world.GetRegistry().EmplaceComponent<SongComponent>(*entity, SongComponent("Level3Sound", true));
            }
            if (gamePacket.entityID == MGE::HashString("Level4Sound")) {
                world.GetRegistry().EmplaceComponent<SongComponent>(*entity, SongComponent("Level4Sound", true));
            }
            break;
        case EntityType::Delete:
            break;
        case EntityType::UI:
            break;
        case EntityType::Life:
            entity->isReplicated = false;
            world.GetRegistry().EmplaceComponent<SpriteComponent>(*entity, SpriteComponent("life", 8, 8));
            if (gamePacket.entityID == MGE::HashString("1")) {
                world.GetRegistry().EmplaceComponent<MGE::Common::TransformComponent>(*entity, MGE::Common::TransformComponent(MGE::Vector3f(5, 5, 0)));
                world.GetRegistry().EmplaceComponent<LifeComponent>(*entity, LifeComponent(1));
            }
            if (gamePacket.entityID == MGE::HashString("2")) {
                world.GetRegistry().EmplaceComponent<MGE::Common::TransformComponent>(*entity, MGE::Common::TransformComponent(MGE::Vector3f(10, 5, 0)));
                world.GetRegistry().EmplaceComponent<LifeComponent>(*entity, LifeComponent(2));
            }
            if (gamePacket.entityID == MGE::HashString("3")) {
                world.GetRegistry().EmplaceComponent<LifeComponent>(*entity, LifeComponent(3));
                world.GetRegistry().EmplaceComponent<MGE::Common::TransformComponent>(*entity, MGE::Common::TransformComponent(MGE::Vector3f(15, 5, 0)));
            }
            std::shared_ptr<MGE::Entity> entity1 = world.GetRegistry().CreateEntity();
            world.GetRegistry().EmplaceComponent<TextComponent>(*entity1, TextComponent("Score: " + std::to_string(world.score), "Title", 50, 5));
            world.GetRegistry().EmplaceComponent<ScoreComponent>(*entity1, ScoreComponent());
            break;
    }
}

void RType::RtypeNetwork::ReceivedServerOrderGameManagement(const ServerOrderGameManagement &gamePacket, MGE::World &world) {
    world.gameState = gamePacket.gameState;
}

void RType::RtypeNetwork::ReceivedLifeUpdateOrderFromServer(LifeSpaceshipUpdate lifeAndScore, MGE::World& world) {
    if (world.client->GetClientId() == lifeAndScore.Player1Score.playerID) {
        world.CurrentLife = lifeAndScore.Player1Score.Life;
        world.score = lifeAndScore.Player1Score.Score;
    }
    if (world.client->GetClientId() == lifeAndScore.Player2Score.playerID) {
        world.CurrentLife = lifeAndScore.Player2Score.Life;
        world.score = lifeAndScore.Player2Score.Score;
    }
    if (world.client->GetClientId() == lifeAndScore.Player4Score.playerID) {
        world.CurrentLife = lifeAndScore.Player4Score.Life;
        world.score = lifeAndScore.Player3Score.Score;
    }
    if (world.client->GetClientId() == lifeAndScore.Player4Score.playerID) {
        world.CurrentLife = lifeAndScore.Player4Score.Life;
        world.score = lifeAndScore.Player4Score.Score;
    }
}

