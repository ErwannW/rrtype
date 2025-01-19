//
// Created by louis on 09/01/2025.
//

#include "ComponentTekken.hpp"

void ComponentRType::SpaceShipSystem(MGE::World &world, MGE::EntityRef& entity, SpaceshipComponent &spaceshipComponent, MGE::Common::TransformComponent &transformComponent)
{
    if constexpr (MGE::BuildTarget::IsServerBuild()) {
        if (spaceshipComponent.isJumping) {
            if (spaceshipComponent.jumpProgress < 0.5f) {
                transformComponent.position.y -= 20 * world.GetFDeltaSeconds();
                spaceshipComponent.jumpProgress += world.GetFDeltaSeconds();
            } else {
                transformComponent.position.y += 20 * world.GetFDeltaSeconds();
                spaceshipComponent.jumpProgress += world.GetFDeltaSeconds();
                if (spaceshipComponent.jumpProgress >= 1) {
                    spaceshipComponent.isJumping = false;
                    transformComponent.position.y = 75;
                    spaceshipComponent.jumpProgress = 0;
                }
            }
        }
        return;
    }

    if (world.GetLocalPlayerId() != entity.entity->owner)
        return;

    const std::map<keyboardKey, KeyState>& pressedKeys = world.engine.GetRenderer<SfmlDisplay>()->getKeyBind();

    for (const auto & [key, state] : pressedKeys) {
        static const std::map<keyboardKey, RType::SpaceShipEventType> keyToEvent = {
            { keyboardKey::A, RType::SpaceShipEventType::Shot },
            { keyboardKey::Space, RType::SpaceShipEventType::Jump },
            { keyboardKey::Arrow_left, RType::SpaceShipEventType::MoveLeft },
            { keyboardKey::Q, RType::SpaceShipEventType::MoveLeft },
            { keyboardKey::Arrow_right, RType::SpaceShipEventType::MoveRight },
            { keyboardKey::D, RType::SpaceShipEventType::MoveRight },
            { keyboardKey::E, RType::SpaceShipEventType::Attack },
        };

        const auto& it = keyToEvent.find(key);

        if (it != keyToEvent.end() && IsPressedOrDown(state)) {
            RType::SpaceShipEvent spaceShipEvent;
            spaceShipEvent.spaceShipEventType = it->second;
            spaceShipEvent.spaceShipEventTarget = entity.entity->GetNetId();
            spaceShipEvent.commandId = MGE::NetCommand::Game;
            spaceShipEvent.gameCommandeID = RType::RtypeNetCommande::SpaceShipEvent;
            RType::RtypeNetwork::SendRPCSpaceShipEvent(&spaceShipEvent, world);
        }
    }
}

SpaceshipComponent::SpaceshipComponent(const bool isMyShip, float speed): speed(speed), isMyShip(isMyShip)
{
  this->lastShoot = 0.0f;
  this->shootCooldown = 0.65f;
}

void SpaceshipComponent::SetupReflection()
{
    Class.RegisterProperty(isMyShip);
    Class.RegisterProperty(speed);
    Class.RegisterProperty(lastShoot);
    Class.RegisterProperty(shootCooldown);
    Class.RegisterProperty(jumpProgress);
}
