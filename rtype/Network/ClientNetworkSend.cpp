//
// Created by louis on 09/01/2025.
//

#include "GameNetwork.hpp"
#include "World.hpp"

void RType::RtypeNetwork::SendRPCGameEventFormClient(const EventType event, MGE::World &world)
{
    GameEvent gameEvent;
    gameEvent.gameCommandeID = RtypeNetCommande::GameEvent;
    gameEvent.commandId = MGE::NetCommand::Game;
    gameEvent.gameEventType = event;
    world.client->Send(&gameEvent, sizeof(GameEvent), 99, true);
}

void RType::RtypeNetwork::SendRPCSpaceShipEvent(const SpaceShipEvent *packet, MGE::World &world)
{
    if (packet == nullptr)
        return;
    packet->commandId = MGE::NetCommand::Game;
    packet->gameCommandeID = RtypeNetCommande::SpaceShipEvent;
    world.client->Send(packet, sizeof(SpaceShipEvent), 99, true);
}