//
// Created by louis on 09/01/2025.
//

#include "SystemeComponent.hpp"

void ComponentRType::SpaceShipSystem(MGE::World &world, MGE::EntityRef& entity, SpaceshipComponent &spaceshipComponent, MGE::Common::TransformComponent &transformComponent)
{
  if (!MGE::BuildTarget::IsClientBuild() || world.GetLocalPlayerId() != entity.entity->owner)
    return;

  const std::map<keyboardKey, KeyState>& pressedKeys = world.engine.GetRenderer<SfmlDisplay>()->getKeyBind();

  for (const auto & [key, state] : pressedKeys) {
    static const std::map<keyboardKey, RType::SpaceShipEventType> keyToEvent = {
      { keyboardKey::Space, RType::SpaceShipEventType::Shot },
      { keyboardKey::Arrow_up, RType::SpaceShipEventType::MoveUp },
      { keyboardKey::Z, RType::SpaceShipEventType::MoveUp },
      { keyboardKey::Arrow_down, RType::SpaceShipEventType::MoveDown },
      { keyboardKey::S, RType::SpaceShipEventType::MoveDown },
      { keyboardKey::Arrow_left, RType::SpaceShipEventType::MoveLeft },
      { keyboardKey::Q, RType::SpaceShipEventType::MoveLeft },
      { keyboardKey::Arrow_right, RType::SpaceShipEventType::MoveRight },
      { keyboardKey::D, RType::SpaceShipEventType::MoveRight },
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

SpaceshipComponent::SpaceshipComponent(bool isMyShip, float speed) : isMyShip(isMyShip), speed(speed)
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
}
