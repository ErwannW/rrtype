//
// Created by louis on 09/01/2025.
//

// ENGINE INCLUDES
#include "../../Engine/World.hpp"
#include "../../Engine/Registry.hpp"

// LOCAL INCLUDES
#include "../Component/ComponentTekken.hpp"
#include "GameNetwork.hpp"

void RType::RtypeNetwork::SendRPCEntity(ManageEntity packet, const MGE::World &world)
{
    if (!MGE::BuildTarget::IsServerBuild())
        return;

    packet.commandId = MGE::NetCommand::Game;
    packet.gameCommandeID = RtypeNetCommande::ManageEntity;

    world.server->Send(&packet, sizeof(ManageEntity), 99, true);
}

void RType::RtypeNetwork::SendRPCServerOrderGameManagement(GameState gameState, const MGE::World &world)
{
    ServerOrderGameManagement serverOrderGameManagement{};
    serverOrderGameManagement.gameState = gameState;
    serverOrderGameManagement.gameCommandeID = RtypeNetCommande::ServerOrderGameManagement;
    serverOrderGameManagement.commandId = MGE::NetCommand::Game;

    world.server->Send(&serverOrderGameManagement, sizeof(ServerOrderGameManagement), 99, true);
}

void RType::RtypeNetwork::SendRPCParalax(NetParalax packet, const MGE::World &world)
{
    if constexpr (!MGE::BuildTarget::IsServerBuild())
        return;

    packet.commandId = MGE::NetCommand::Game;
    packet.gameCommandeID = RtypeNetCommande::Paralax;

    world.server->Send(&packet, sizeof(ManageEntity), 99, true);
}
