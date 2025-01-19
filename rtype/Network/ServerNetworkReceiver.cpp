//
// Created by louis on 09/01/2025.
//

#include "GameNetwork.hpp"
#include "Registry.hpp"
#include "../component/SystemeComponent.hpp"
#include "Commons/Components/VelocityComponent.hpp"
#include "World.hpp"

void RType::RtypeNetwork::ReceivedGameEventFromClient(const GameEvent &gamePacket,  uint16 clientId, MGE::World &world)
{

    if (&gamePacket == nullptr)
		return;
    std::vector<Player>& players = world.GetPlayers();
    for (auto & player : players) {
      if (player.playerId == clientId) {
        if (gamePacket.gameEventType == EventType::NextStage) {
          player.gameEvent = EventType::InGame;
          world.howPlayerContinueToPlay += 1;
        } else {
          player.gameEvent = gamePacket.gameEventType;
        }
      }
    }
}

void RType::RtypeNetwork::ReceivedRPCSpaceShipEvent(const SpaceShipEvent &gamePacket, uint16 clientID, MGE::World &world)
{
    if (&gamePacket == nullptr)
        return;

    std::shared_ptr<MGE::Entity> spaceShip = world.GetRegistry().GetEntityByNetID(gamePacket.spaceShipEventTarget);
    if (!MGE::Entity::IsValid(spaceShip))
        return;

    if (!CHECK(spaceShip->owner == clientID, "A client is attempting to modify the properties of an entity that does not belong to them!"))
        return;
    auto &spaceShipPositions = world.GetRegistry().GetComponent<MGE::Common::TransformComponent>(*spaceShip);
    auto &spaceShipConf = world.GetRegistry().GetComponent<SpaceshipComponent>(*spaceShip);
    //spaceShipPositions.position.y += 20.f * World::GetFDeltaSeconds();
    switch (gamePacket.spaceShipEventType) {
        case SpaceShipEventType::MoveDown:
            spaceShipPositions.position.y += spaceShipConf.speed;
            if (spaceShipPositions.position.y >= 100) {
                spaceShipPositions.position.y = 0;
            }
            break;
        case SpaceShipEventType::MoveUp:
            spaceShipPositions.position.y -= spaceShipConf.speed;
            if (spaceShipPositions.position.y <= 0) {
                spaceShipPositions.position.y = 100;
            }
            break;
        case SpaceShipEventType::MoveLeft:
            spaceShipPositions.position.x -= spaceShipConf.speed;
            if (spaceShipPositions.position.x <= 5) {
                spaceShipPositions.position.x = 5;
            }
            break;
        case SpaceShipEventType::MoveRight:
            spaceShipPositions.position.x += spaceShipConf.speed;
            if (spaceShipPositions.position.x >= 50) {
                spaceShipPositions.position.x = 50;
            }
            break;
        case SpaceShipEventType::Shot: {
            auto currentTime = std::chrono::duration<float>(std::chrono::steady_clock::now().time_since_epoch()).count();
            auto spaceShipPosition = world.GetRegistry().GetComponent<MGE::Common::TransformComponent>(*spaceShip);

            if (currentTime - spaceShipConf.lastShoot >= spaceShipConf.shootCooldown) {
                spaceShipConf.lastShoot = currentTime;

                std::shared_ptr<MGE::Entity> projectile = world.GetRegistry().CreateEntity();
                projectile->SetNetId(world.GetNewUniqueNetID());
                projectile->isReplicated = true;
                projectile->owner = clientID;

                world.GetRegistry().EmplaceComponent<ProjectileComponent>(projectile, ProjectileComponent(0, 0, 1));
                world.GetRegistry().EmplaceComponent<MGE::Common::VelocityComponent>(projectile, 40.f, 0.f, 0.f);
                world.GetRegistry().EmplaceComponent<MGE::Common::TransformComponent>(projectile, MGE::Common::TransformComponent(MGE::Vector3f(spaceShipPosition.position.x, spaceShipPosition.position.y, 0)));

                ManageEntity createPlayerEntityPacket(spaceShipPosition.position, EntityType::Bullet,
                  MGE::HashString("player_base_projectile"), projectile->GetNetId(), projectile->owner);
                SendRPCEntity(createPlayerEntityPacket, world);
            }
            break;
        }
        default:
            world.engine.GetLogger()->Error("Unknown spaceShipEvent type received from a player!");
            return;
    }
}


