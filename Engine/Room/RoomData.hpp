//
// Created by louis on 13/01/2025.
//

#ifndef ROOMDATA_HPP
#define ROOMDATA_HPP

#include "RoomPacket.hpp"
#include <vector>
#include <string>
#include <queue>

namespace MGE {

  class World;

  struct ReturnData {
    bool isSucces;
    std::string message;
  };

  class RoomData {
    public:
      RoomData() {}
      ~RoomData() {}

      ReturnData CreateNewRoom(uint16_t maxPlayer, uint16_t clientID, std::string roomName);
      ReturnData DestroyRoom(uint16_t roomID, uint16_t clientID);
      ReturnData JoinRoom(uint16_t roomID, uint16_t clientID);
      ReturnData LeaveRoom(uint16_t roomID, uint16_t clientID);


      static void ResponseMessageManager(ReturnData returnData, const World& world);
      static void SendSuccessRoomDataRPC(const std::string &message, const World& world);
      static void SendFailureRoomDataRPC(const std::string &message, const World &world);
      static void SendCreateRoomDataRPC(const std::string &roomName, uint16_t maxPlayers, const World& world);
      static void SendJoinRequestDataRPC(const uint16_t &roomID, const World &world);
      static void SendLeaveRequestDataRPC(const uint16_t &roomID, const World& world);
      static void SendDestroyRoomDataRPC(const uint16_t &roomID, const World &world);
      static void SendRequestListDataRPC(const World &world);
      void SendListRoomDataRPC(const World& world);
      std::vector<RoomDataStruct> GetRoomData();
      void ReceiveRoomDataRPC(const RoomPacket::RoomGestionPacket * packet, uint16_t uint16, const World &world);
      std::queue<ReturnData> &GetReturnData();
    bool receiveNewList = false;
  private:
        std::vector<RoomDataStruct> allRoomsData;
        std::queue<ReturnData> roomsDataResponse;
  };
}

#endif //ROOMDATA_HPP
