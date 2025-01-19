//
// Created by louis on 09/01/2025.
//

#include <iostream>

// LOCAL INCLUDES
#include "GameNetwork.hpp"

void RType::RtypeNetwork::GameNetworkReceiver(MGE::World& world, const GameNetwork* GamePacket, uint16 clientId) {
    const ManageEntity* manageEntity = nullptr;
    const SpaceShipEvent* spaceShipEvent = nullptr;
    const GameEvent* gameEvent = nullptr;
    const ServerOrderGameManagement* serverOrderGameManagement = nullptr;
    const NetParalax* netParalax = nullptr;
    switch (GamePacket->gameCommandeID) {
    case RtypeNetCommande::ManageEntity:
        manageEntity = reinterpret_cast<const ManageEntity *>(GamePacket);
        if (manageEntity) {
            ReceivedRPCEntity(*manageEntity, world);
        } else {
            world.engine.GetLogger<>()->Error("Failed to cast GamePacket to ManageEntity.");
        }
        break;
    case RtypeNetCommande::SpaceShipEvent:
        spaceShipEvent = reinterpret_cast<const SpaceShipEvent *>(GamePacket);
        if (spaceShipEvent) {
            ReceivedRPCSpaceShipEvent(*spaceShipEvent, clientId, world);
        } else {
            world.engine.GetLogger<>()->Error("Failed to cast GamePacket to SpaceShipEvent.");
        }
        break;
    case RtypeNetCommande::GameEvent:
        gameEvent = reinterpret_cast<const GameEvent *>(GamePacket);
        if (gameEvent) {
            ReceivedGameEventFromClient(*gameEvent, clientId, world);
        } else {
            world.engine.GetLogger<>()->Error("Failed to cast GamePacket to GameEvent.");
        }
        break;
    case RtypeNetCommande::ServerOrderGameManagement:
        serverOrderGameManagement = reinterpret_cast<const ServerOrderGameManagement *>(GamePacket);
        if (serverOrderGameManagement) {
            ReceivedServerOrderGameManagement(*serverOrderGameManagement, world);
        } else {
            world.engine.GetLogger<>()->Error("Failed to cast GamePacket to ServerOrderGameManagement.");
        }
        break;
    case RtypeNetCommande::Paralax:
        netParalax = reinterpret_cast<const NetParalax *>(GamePacket);
        if (netParalax) {
            ReceivedParalaxOrderFromServer(*netParalax, world);
        } else {
            world.engine.GetLogger<>()->Error("Failed to cast GamePacket to NetParalax.");
        }
        break;
    default:
        world.engine.GetLogger<>()->Error("Unknown GamePacket.");
    }
}
