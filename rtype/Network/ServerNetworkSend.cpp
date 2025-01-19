//
// Created by louis on 09/01/2025.
//

#include "GameNetwork.hpp"
#include "World.hpp"
#include "Registry.hpp"
#include "../component/SystemeComponent.hpp"

void RType::RtypeNetwork::SendRPCEntity(ManageEntity packet, const MGE::World &world)
{
      if (!MGE::BuildTarget::IsServerBuild())
        return;

      packet.commandId = MGE::NetCommand::Game;
      packet.gameCommandeID = RtypeNetCommande::ManageEntity;

      world.server->Send(&packet, sizeof(ManageEntity), 99, true);
}

bool RType::RtypeNetwork::SendRPCServerOrderGameManagement(GameState gameState, const MGE::World &world) {
      ServerOrderGameManagement serverOrderGameManagement{};
      serverOrderGameManagement.gameState = gameState;
      serverOrderGameManagement.gameCommandeID = RtypeNetCommande::ServerOrderGameManagement;
      serverOrderGameManagement.commandId = MGE::NetCommand::Game;

      world.server->Send(&serverOrderGameManagement, sizeof(ServerOrderGameManagement), 99, true);
      return true;
}

void RType::RtypeNetwork::SendRPCParalax(NetParalax packet, const MGE::World &world)
{
      if (!MGE::BuildTarget::IsServerBuild())
        return;

      packet.commandId = MGE::NetCommand::Game;
      packet.gameCommandeID = RtypeNetCommande::Paralax;
      world.server->Send(&packet, sizeof(ManageEntity), 99, true);
}

void RType::RtypeNetwork::SendRPCLifeUpdate(const MGE::World &world)
{
    if (!MGE::BuildTarget::IsServerBuild())
      return;

    LifeSpaceshipUpdate lifeSpaceShipUpdate{GameNetwork(RtypeNetCommande::LifeUpdate)};
    lifeSpaceShipUpdate.commandId = MGE::NetCommand::Game;

    int nbPlayer = world.GetPlayers().size();
    if (nbPlayer == 1) {
        lifeSpaceShipUpdate.Player1Score.Life = world.GetPlayers()[0].life;
        lifeSpaceShipUpdate.Player1Score.Score = world.GetPlayers()[0].score;
        lifeSpaceShipUpdate.Player1Score.playerID = world.GetPlayers()[0].playerId;
    } else {
        lifeSpaceShipUpdate.Player1Score.Life = -1;
        lifeSpaceShipUpdate.Player1Score.Score = -1;
        lifeSpaceShipUpdate.Player1Score.playerID = -1;
    }
    if (nbPlayer == 2) {
        lifeSpaceShipUpdate.Player2Score.Life = world.GetPlayers()[1].life;
        lifeSpaceShipUpdate.Player2Score.Score = world.GetPlayers()[1].score;
        lifeSpaceShipUpdate.Player2Score.playerID = world.GetPlayers()[1].playerId;
    } else {
        lifeSpaceShipUpdate.Player2Score.Life = -1;
        lifeSpaceShipUpdate.Player2Score.Score = -1;
        lifeSpaceShipUpdate.Player2Score.playerID = -1;
    }
    if (nbPlayer == 3) {
        lifeSpaceShipUpdate.Player3Score.Life = world.GetPlayers()[2].life;
        lifeSpaceShipUpdate.Player3Score.Score = world.GetPlayers()[2].score;
        lifeSpaceShipUpdate.Player3Score.playerID = world.GetPlayers()[2].playerId;
    } else {
        lifeSpaceShipUpdate.Player3Score.Life = -1;
        lifeSpaceShipUpdate.Player3Score.Score = -1;
        lifeSpaceShipUpdate.Player3Score.playerID = -1;
    }
    if (nbPlayer == 4) {
        lifeSpaceShipUpdate.Player4Score.Life = world.GetPlayers()[3].life;
        lifeSpaceShipUpdate.Player4Score.Score = world.GetPlayers()[3].score;
        lifeSpaceShipUpdate.Player4Score.playerID = world.GetPlayers()[3].playerId;
    } else {
        lifeSpaceShipUpdate.Player4Score.Life = -1;
        lifeSpaceShipUpdate.Player4Score.Score = -1;
        lifeSpaceShipUpdate.Player4Score.playerID = -1;
    }
    std::future<void> result = std::async(std::launch::async, [&]() {
        world.server->Send(&lifeSpaceShipUpdate, sizeof(LifeSpaceshipUpdate), 99, true);
    });
    result.get();
}
