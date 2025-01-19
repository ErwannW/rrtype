//
// Created by louis on 13/01/25.
//

#ifndef ROOMPACKET_HPP
#define ROOMPACKET_HPP

#include "Network/Packets/NetPacket.hpp"
#include "Hashing/Hash.hpp"
#include <vector>
#include <string.h>
#include <array>

static constexpr std::size_t MaxMessageSize = 64;

namespace MGE {
    struct RoomDataStruct
    {
      uint16_t roomID;
      uint16_t maxPlayer;
      uint16_t TheCreatorOfThisRoomIs;
      std::vector<int> playersEnroled;
      char roomName[MaxMessageSize];
    };

    struct RoomDataStructNetwork
    {
      uint16_t roomID;
      uint16_t maxPlayer;
      uint16_t TheCreatorOfThisRoomIs;
      int player1;
      int player2;
      int player3;
      int player4;
      char roomName[MaxMessageSize];
    };

    namespace RoomPacket {
        enum class RoomGestionType {
          RequestJoin,
          RequestLeave,
          RequestCreate,
          RequestDestroy,
          RequestList,
          RequestSuccess,
          RequestFailed,
          RespondList,
        };

        struct RoomGestionPacket : NetPacket
        {
          RoomGestionType type;
        };

        struct RoomGestionRequestJoinPacket : RoomGestionPacket
        {
          int id;
        };

        struct RoomGestionRequestLeavePacket : RoomGestionPacket
        {
          int id;
        };

        struct RoomGestionRequestCreatePacket : RoomGestionPacket
        {
          char roomName[MaxMessageSize];
          int maxPlayers;
        };

        struct RoomGestionRequestDestroyPacket : RoomGestionPacket
        {
          int id;
        };

        struct RoomGestionRespondListPacket : RoomGestionPacket
        {
          mutable int nbRoom;
          RoomDataStructNetwork room1;
          RoomDataStructNetwork room2;
          RoomDataStructNetwork room3;
          RoomDataStructNetwork room4;
          RoomDataStructNetwork room5;
        };

        struct RoomGestionRequestInfoPacket : RoomGestionPacket
        {
          int id;
          int nbPlayersEnroled;
          int maxPlayers;
          char roomName[MaxMessageSize];
        };

        struct RoomGestionRequestFailedPacket : RoomGestionPacket
        {
          char message[MaxMessageSize];
        };

        struct RoomGestionRequestSuccessPacket : RoomGestionPacket
        {
          char message[MaxMessageSize];
        };
    }
} // MGE

#endif //ROOMPACKET_HPP
