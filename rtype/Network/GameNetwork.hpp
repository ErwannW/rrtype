//
// Created by louis on 09/01/2025.
//

#ifndef NETWORK_H
#define NETWORK_H

#include "../../Engine/Engine.hpp"
#include "../../Engine/World.hpp"
#include "GameNetworkEnum.hpp"
#include "Network/Packets/NetPacket.hpp"
#include "Hashing/Hash.hpp"
#include "Vector/Vector3.hpp"

namespace RType {


    struct GameNetwork : public MGE::NetPacket {
      mutable RtypeNetCommande gameCommandeID;

      explicit GameNetwork(RtypeNetCommande commande)
          : gameCommandeID(commande) {}
    };


    struct LifeAndScore {
      uint16_t Life = 0;
      uint16_t Score = 0;
      uint16_t playerID = 0;
    };

    struct LifeSpaceshipUpdate : public GameNetwork {
      LifeAndScore Player1Score;
      LifeAndScore Player2Score;
      LifeAndScore Player3Score;
      LifeAndScore Player4Score;
    };


    /**
    * @brief Packet sent by the server to all client to create a new entity.
    */
    struct ManageEntity : public GameNetwork {
      MGE::Vector3f entityPosition;
      EntityType entityType;
      MGE::Hash entityID;
      MGE::NetId netId;
      uint16_t playerID{};

      ManageEntity(MGE::Vector3f pos, EntityType type, MGE::Hash id, MGE::NetId netid, uint16_t player = 0)
        : GameNetwork(RtypeNetCommande::ManageEntity), entityPosition(pos), entityType(type), entityID(id),
          netId(netid), playerID(player) {}
    };

    /**
    * @brief Structure representing a packet for handling parallax effects in the game.
    *
    * This structure contains the necessary information to create a parallax effect using two different sprites
    * and their respective speeds. It is used to send information to clients to render the visual effect.
    */
    struct NetParalax : public GameNetwork {
      MGE::Hash spriteID1; ///< ID of the first sprite
      float speed1;         ///< Speed of the first sprite
      MGE::Hash spriteID2; ///< ID of the second sprite
      float speed2;         ///< Speed of the second sprite

      /**
      * @brief Constructor for the NetParalax structure.
      *
      * Initializes the members with default values.
      */
      explicit NetParalax(MGE::Hash sprite1 = MGE::Hash(), float spd1 = 0.0f, MGE::Hash sprite2 = MGE::Hash(), float spd2 = 0.0f)
        : GameNetwork(RtypeNetCommande::Paralax), spriteID1(sprite1), speed1(spd1), spriteID2(sprite2),
          speed2(spd2) {
      }
    };

    /**
    * @brief Event sent by the client to the server to move the spaceship or perform a shot.
    *
    * This structure is used to send a specific event to the server, such as moving the spaceship
    * or performing a shot. It is sent by the client to update the spaceship's state in the game.
    */
    struct SpaceShipEvent : public GameNetwork {
      SpaceShipEventType spaceShipEventType; ///< Type of event related to the spaceship (move, shot, etc.)
      MGE::NetId spaceShipEventTarget;       ///< Target of the event (the entity to affect)

      /**
      * @brief Constructor for the SpaceShipEvent structure.
      *
      * Initializes the members with default values.
      */
      explicit SpaceShipEvent(SpaceShipEventType eventType = SpaceShipEventType::Shot, MGE::NetId target = MGE::NetId())
        : GameNetwork(RtypeNetCommande::SpaceShipEvent), spaceShipEventType(eventType),
          spaceShipEventTarget(target) {
      }
    };

    /**
    * @brief Event sent by the client to the server regarding general game events.
    *
    * This structure is used to handle events in the game such as state changes, level progression, etc.
    * It is sent by the client to inform the server of the player's actions.
    */
    struct GameEvent : public GameNetwork {
      EventType gameEventType; ///< Type of the event in the game (Lobby, InGame, etc.)

      /**
      * @brief Constructor for the GameEvent structure.
      *
      * Initializes the members with default values.
      */
      explicit GameEvent(EventType eventType = EventType::Lobby)
        : GameNetwork(RtypeNetCommande::GameEvent), gameEventType(eventType) {
      }
    };

    /**
    * @brief Game state, used to manage server orders.
    *
    * This structure allows managing the current state of the game (Loading, InGame, GameOver, etc.)
    * and sends information to the client to inform them about the game's current state.
    */
    struct ServerOrderGameManagement : public GameNetwork {
      GameState gameState; ///< The current game state (Loading, InGame, etc.)

      /**
      * @brief Constructor for the ServerOrderGameManagement structure.
      *
      * Initializes the members with default values.
      */
      explicit ServerOrderGameManagement(GameState state = GameState::Loading)
        : GameNetwork(RtypeNetCommande::ServerOrderGameManagement), gameState(state) {
      }
    };

    class RtypeNetwork {
    public:
      RtypeNetwork() = default;
      ~RtypeNetwork() = default;


      static void ReceivedLifeUpdateOrderFromServer(LifeSpaceshipUpdate lifeAndScore, MGE::World &world);

      static void GameNetworkReceiver(MGE::World& world, const GameNetwork* GamePacket, uint16 clientId);
      static void ReceivedRPCEntity(const ManageEntity& gamePacket, MGE::World& world);

      static void ReceivedRPCSpaceShipEvent(const SpaceShipEvent &gamePacket, uint16 clientId, MGE::World &world);
      static void ReceivedGameEventFromClient(const GameEvent &gamePacket, uint16 clientId, MGE::World &world);
      static void ReceivedServerOrderGameManagement(const ServerOrderGameManagement &gamePacket, MGE::World &world);
      static void ReceivedParalaxOrderFromServer(const NetParalax &gamePacket, MGE::World &world);

      static void SendRPCEntity(ManageEntity packet, const MGE::World &world);

      static void SendRPCLifeUpdate(const MGE::World &world);

      static bool SendRPCServerOrderGameManagement(GameState gameState, const MGE::World &world);
      static void SendRPCSpaceShipEvent(const SpaceShipEvent *packet, MGE::World &world);
      static void SendRPCGameEventFormClient(const EventType event, MGE::World &world);
      static void SendRPCParalax(NetParalax packet, const MGE::World &world);

    };
}
#endif //NETWORK_H
