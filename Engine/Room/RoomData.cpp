//
// Created by louis on 13/01/2025.
//

#include "RoomData.hpp"
#include <BuildTarget.hpp>
#include <iostream>
#include "World.hpp"
#include <string.h>

MGE::ReturnData MGE::RoomData::CreateNewRoom(const uint16_t maxPlayer, const uint16_t clientID, std::string roomName) {
    ReturnData result;

    if (allRoomsData.size() >= 5) {
    result.isSucces = false;
    result.message = "Room limit reached. Cannot create more rooms.";
    return result;
    }

    RoomDataStruct newRoom;
    newRoom.roomID = static_cast<uint16_t>(allRoomsData.size() + 1);
    newRoom.maxPlayer = maxPlayer;
    newRoom.TheCreatorOfThisRoomIs = clientID;
    std::strncpy(newRoom.roomName, roomName.c_str(), sizeof(newRoom.roomName) - 1);
    newRoom.roomName[sizeof(newRoom.roomName) - 1] = '\0';
    allRoomsData.push_back(newRoom);

    result.isSucces = true;
    result.message = "Room created successfully.";
    return result;
}

MGE::ReturnData MGE::RoomData::DestroyRoom(const uint16_t roomID, const uint16_t clientID) {
    ReturnData result;

    for (auto it = allRoomsData.begin(); it != allRoomsData.end(); ++it) {
        if (it->roomID == roomID) {
            if (it->TheCreatorOfThisRoomIs == clientID) {
                allRoomsData.erase(it);
                result.isSucces = true;
                result.message = "Room destroyed successfully.";
                return result;
            } else {
                result.isSucces = false;
                result.message = "Only the creator can destroy the room.";
                return result;
            }
        }
    }

    result.isSucces = false;
    result.message = "Room not found.";
    return result;
}

MGE::ReturnData MGE::RoomData::JoinRoom(const uint16_t roomID, const uint16_t clientID) {
    ReturnData result;

    for (const auto& roomData : allRoomsData) {
        if (std::find(roomData.playersEnroled.begin(), roomData.playersEnroled.end(), clientID) != roomData.playersEnroled.end()) {
            result.isSucces = false;
            result.message = "You are already enrolled in a room.";
            return result;
        }
    }

    for (auto& roomData : allRoomsData) {
        if (roomData.roomID == roomID) {
            if (roomData.playersEnroled.size() < roomData.maxPlayer) {
                roomData.playersEnroled.push_back(clientID);
                result.isSucces = true;
                result.message = "Successfully joined the room.";
                return result;
            } else {
                result.isSucces = false;
                result.message = "The room is full.";
                return result;
            }
        }
    }

    result.isSucces = false;
    result.message = "Room not found.";
    return result;
}

MGE::ReturnData MGE::RoomData::LeaveRoom(const uint16_t roomID, const uint16_t clientID) {
    ReturnData result;

    for (auto& roomData : allRoomsData) {
        if (roomData.roomID == roomID) {
            for (auto it = roomData.playersEnroled.begin(); it != roomData.playersEnroled.end(); ++it) {
                if (*it == clientID) {
                    roomData.playersEnroled.erase(it);
                    result.isSucces = true;
                    result.message = "Successfully left the room.";
                    return result;
                }
            }
            result.isSucces = false;
            result.message = "You are not enrolled in this room.";
            return result;
        }
    }
    result.isSucces = false;
    result.message = "Room not found.";
    return result;
}

std::vector<MGE::RoomDataStruct> MGE::RoomData::GetRoomData() {
  return allRoomsData;
}

void MGE::RoomData::ReceiveRoomDataRPC(const RoomPacket::RoomGestionPacket *packet, uint16_t clientID, const World &world) {
    if (BuildTarget::IsClientBuild()) {
        if (packet->type == RoomPacket::RoomGestionType::RequestSuccess) {
            const RoomPacket::RoomGestionRequestSuccessPacket* requestSuccess = nullptr;
            requestSuccess = reinterpret_cast<const RoomPacket::RoomGestionRequestSuccessPacket *>(packet);
            if (requestSuccess) {
                ReturnData result;
                result.isSucces = true;
                result.message = requestSuccess->message;
                roomsDataResponse.emplace(result);
            }
        } else if (packet->type == RoomPacket::RoomGestionType::RequestFailed) {
            const RoomPacket::RoomGestionRequestFailedPacket* requestFailed = nullptr;
            requestFailed = reinterpret_cast<const RoomPacket::RoomGestionRequestFailedPacket *>(packet);
            if (requestFailed) {
                roomsDataResponse.emplace(ReturnData(false, requestFailed->message));
            }
        } else if (packet->type == RoomPacket::RoomGestionType::RespondList) {
            const RoomPacket::RoomGestionRespondListPacket* responseList = nullptr;
            responseList = reinterpret_cast<const RoomPacket::RoomGestionRespondListPacket *>(packet);
            if (responseList) {
                receiveNewList = true;
                allRoomsData.clear();
                allRoomsData.resize(responseList->nbRoom);
                if (responseList->nbRoom >= 1) {
                    allRoomsData[0].maxPlayer = responseList->room1.maxPlayer;
                    strcpy(allRoomsData[0].roomName, responseList->room1.roomName);
                    allRoomsData[0].TheCreatorOfThisRoomIs = responseList->room1.TheCreatorOfThisRoomIs;
                    allRoomsData[0].roomID = responseList->room1.roomID;
                    if (responseList->room1.player1 != -1)
                        allRoomsData[0].playersEnroled.push_back(responseList->room1.player1);
                    if (responseList->room1.player2 != -1)
                        allRoomsData[0].playersEnroled.push_back(responseList->room1.player2);
                    if (responseList->room1.player3 != -1)
                        allRoomsData[0].playersEnroled.push_back(responseList->room1.player3);
                    if (responseList->room1.player4 != -1)
                        allRoomsData[0].playersEnroled.push_back(responseList->room1.player4);
                }
                if (responseList->nbRoom >= 2) {
                    allRoomsData[1].maxPlayer = responseList->room2.maxPlayer;
                    strcpy(allRoomsData[1].roomName, responseList->room2.roomName);
                    allRoomsData[1].TheCreatorOfThisRoomIs = responseList->room2.TheCreatorOfThisRoomIs;
                    allRoomsData[1].roomID = responseList->room2.roomID;
                    if (responseList->room2.player1 != -1)
                        allRoomsData[1].playersEnroled.push_back(responseList->room2.player1);
                    if (responseList->room2.player2 != -1)
                        allRoomsData[1].playersEnroled.push_back(responseList->room2.player2);
                    if (responseList->room2.player3 != -1)
                        allRoomsData[1].playersEnroled.push_back(responseList->room2.player3);
                    if (responseList->room2.player4 != -1)
                        allRoomsData[1].playersEnroled.push_back(responseList->room2.player4);
                }
                if (responseList->nbRoom >= 3) {
                    allRoomsData[2].maxPlayer = responseList->room3.maxPlayer;
                    strcpy(allRoomsData[2].roomName, responseList->room3.roomName);
                    allRoomsData[2].TheCreatorOfThisRoomIs = responseList->room3.TheCreatorOfThisRoomIs;
                    allRoomsData[2].roomID = responseList->room3.roomID;
                    if (responseList->room3.player1 != -1)
                        allRoomsData[2].playersEnroled.push_back(responseList->room3.player1);
                    if (responseList->room3.player2 != -1)
                        allRoomsData[2].playersEnroled.push_back(responseList->room3.player2);
                    if (responseList->room3.player3 != -1)
                        allRoomsData[2].playersEnroled.push_back(responseList->room3.player3);
                    if (responseList->room3.player4 != -1)
                        allRoomsData[2].playersEnroled.push_back(responseList->room3.player4);
                }
                if (responseList->nbRoom >= 4) {
                    allRoomsData[3].maxPlayer = responseList->room4.maxPlayer;
                    strcpy(allRoomsData[3].roomName, responseList->room4.roomName);
                    allRoomsData[3].TheCreatorOfThisRoomIs = responseList->room4.TheCreatorOfThisRoomIs;
                    allRoomsData[3].roomID = responseList->room4.roomID;
                    if (responseList->room4.player1 != -1)
                        allRoomsData[3].playersEnroled.push_back(responseList->room4.player1);
                    if (responseList->room4.player2 != -1)
                        allRoomsData[3].playersEnroled.push_back(responseList->room4.player2);
                    if (responseList->room4.player3 != -1)
                        allRoomsData[3].playersEnroled.push_back(responseList->room4.player3);
                    if (responseList->room4.player4 != -1)
                        allRoomsData[3].playersEnroled.push_back(responseList->room4.player4);
                }
                if (responseList->nbRoom == 5) {
                    allRoomsData[4].maxPlayer = responseList->room5.maxPlayer;
                    strcpy(allRoomsData[4].roomName, responseList->room5.roomName);
                    allRoomsData[4].TheCreatorOfThisRoomIs = responseList->room5.TheCreatorOfThisRoomIs;
                    allRoomsData[4].roomID = responseList->room5.roomID;
                    if (responseList->room5.player1 != -1)
                        allRoomsData[4].playersEnroled.push_back(responseList->room5.player1);
                    if (responseList->room5.player2 != -1)
                        allRoomsData[4].playersEnroled.push_back(responseList->room5.player2);
                    if (responseList->room5.player3 != -1)
                        allRoomsData[4].playersEnroled.push_back(responseList->room5.player3);
                    if (responseList->room5.player4 != -1)
                        allRoomsData[4].playersEnroled.push_back(responseList->room5.player4);
                }
                responseList->nbRoom = 0;
            }
        }
    } else if (BuildTarget::IsServerBuild()) {
      const RoomPacket::RoomGestionRequestCreatePacket* requestCreate = nullptr;
      const RoomPacket::RoomGestionRequestJoinPacket* requestJoin = nullptr;
      const RoomPacket::RoomGestionRequestLeavePacket* requestLeave = nullptr;
      const RoomPacket::RoomGestionRequestDestroyPacket* requestDestroy = nullptr;

      switch (packet->type) {
      case RoomPacket::RoomGestionType::RequestCreate:
        requestCreate = reinterpret_cast<const RoomPacket::RoomGestionRequestCreatePacket *>(packet);
        if (requestCreate) {
          ResponseMessageManager(CreateNewRoom(requestCreate->maxPlayers, clientID, requestCreate->roomName), world);
        }
        break;
      case RoomPacket::RoomGestionType::RequestJoin:
        requestJoin = reinterpret_cast<const RoomPacket::RoomGestionRequestJoinPacket *>(packet);
        if (requestJoin) {
          ResponseMessageManager(JoinRoom(requestJoin->id, clientID), world);
        }
        break;
      case RoomPacket::RoomGestionType::RequestLeave:
        requestLeave = reinterpret_cast<const RoomPacket::RoomGestionRequestLeavePacket *>(packet);
        if (requestLeave) {
          ResponseMessageManager(LeaveRoom(requestLeave->id, clientID), world);
        }
        break;
      case RoomPacket::RoomGestionType::RequestDestroy:
        requestDestroy = reinterpret_cast<const RoomPacket::RoomGestionRequestDestroyPacket *>(packet);
        if (requestDestroy) {
          ResponseMessageManager(DestroyRoom(requestDestroy->id, clientID), world);
        }
        break;
      case RoomPacket::RoomGestionType::RequestList:
        SendListRoomDataRPC(world);
        break;
      default:
        break;
      }
    }
}


void MGE::RoomData::SendSuccessRoomDataRPC(const std::string &message, const World &world) {
    RoomPacket::RoomGestionRequestSuccessPacket requestSuccess;
    std::strncpy(requestSuccess.message, message.c_str(), sizeof(requestSuccess.message) - 1);
    requestSuccess.message[sizeof(requestSuccess.message) - 1] = '\0';
    requestSuccess.type = RoomPacket::RoomGestionType::RequestSuccess;
    requestSuccess.commandId = NetCommand::RoomGestion;
    world.server->Send(&requestSuccess, sizeof(RoomPacket::RoomGestionRequestSuccessPacket), 99, true);
}

void MGE::RoomData::SendFailureRoomDataRPC(const std::string &message, const World &world) {
    RoomPacket::RoomGestionRequestFailedPacket requestFail;
    std::strncpy(requestFail.message, message.c_str(), sizeof(requestFail.message) - 1);
    requestFail.message[sizeof(requestFail.message) - 1] = '\0';
    requestFail.type = RoomPacket::RoomGestionType::RequestFailed;
    requestFail.commandId = NetCommand::RoomGestion;
    world.server->Send(&requestFail, sizeof(RoomPacket::RoomGestionRequestFailedPacket), 99, true);
}

void MGE::RoomData::SendCreateRoomDataRPC(const std::string &roomName, const uint16_t maxPlayers, const World &world) {

    RoomPacket::RoomGestionRequestCreatePacket requestCreate;
    requestCreate.maxPlayers = maxPlayers;
    std::strncpy(requestCreate.roomName, roomName.c_str(), sizeof(requestCreate.roomName) - 1);
    requestCreate.roomName[sizeof(requestCreate.roomName) - 1] = '\0';
    requestCreate.type = RoomPacket::RoomGestionType::RequestCreate;
    requestCreate.commandId = NetCommand::RoomGestion;
    world.client->Send(&requestCreate, sizeof(RoomPacket::RoomGestionRequestCreatePacket), 99, true);
}

void MGE::RoomData::SendJoinRequestDataRPC(const uint16_t &roomID, const World &world) {
    RoomPacket::RoomGestionRequestJoinPacket requesJoin;
    requesJoin.id = roomID;
    requesJoin.type = RoomPacket::RoomGestionType::RequestJoin;
    requesJoin.commandId = NetCommand::RoomGestion;
    world.client->Send(&requesJoin, sizeof(RoomPacket::RoomGestionRequestJoinPacket), 99, true);
}

void MGE::RoomData::SendLeaveRequestDataRPC(const uint16_t &roomID, const World &world) {
    RoomPacket::RoomGestionRequestLeavePacket requestLeave;
    requestLeave.id = roomID;
    requestLeave.type = RoomPacket::RoomGestionType::RequestLeave;
    requestLeave.commandId = NetCommand::RoomGestion;
    world.client->Send(&requestLeave, sizeof(RoomPacket::RoomGestionRequestLeavePacket), 99, true);
}

void MGE::RoomData::SendDestroyRoomDataRPC(const uint16_t &roomID, const World &world) {
    RoomPacket::RoomGestionRequestDestroyPacket requestFail;
    requestFail.id = roomID;
    requestFail.type = RoomPacket::RoomGestionType::RequestDestroy;
    requestFail.commandId = NetCommand::RoomGestion;
    world.client->Send(&requestFail, sizeof(RoomPacket::RoomGestionRequestDestroyPacket), 99, true);
}

void MGE::RoomData::SendRequestListDataRPC(const World &world) {
    RoomPacket::RoomGestionPacket requestList;
    requestList.type = RoomPacket::RoomGestionType::RequestList;
    requestList.commandId = NetCommand::RoomGestion;
    world.client->Send(&requestList, sizeof(RoomPacket::RoomGestionPacket), 99, true);
}

void MGE::RoomData::SendListRoomDataRPC(const World &world) {
    RoomPacket::RoomGestionRespondListPacket respondList;
    if (allRoomsData.size() >= 1) {
        respondList.room1.maxPlayer = allRoomsData[0].maxPlayer;
        strcpy(respondList.room1.roomName, allRoomsData[0].roomName);
        respondList.room1.roomID = allRoomsData[0].roomID;
        respondList.room1.TheCreatorOfThisRoomIs = allRoomsData[0].TheCreatorOfThisRoomIs;
        respondList.room1.player1 = -1;
        respondList.room1.player2 = -1;
        respondList.room1.player3 = -1;
        respondList.room1.player4 = -1;
        if (allRoomsData[0].playersEnroled.size() >= 1)
            respondList.room1.player1 = allRoomsData[0].playersEnroled[0];
        if (allRoomsData[0].playersEnroled.size() >= 2)
            respondList.room1.player2 = allRoomsData[0].playersEnroled[1];
        if (allRoomsData[0].playersEnroled.size() >= 3)
            respondList.room1.player3 = allRoomsData[0].playersEnroled[2];
        if (allRoomsData[0].playersEnroled.size() >= 4)
            respondList.room1.player4 = allRoomsData[0].playersEnroled[3];
    }
    if (allRoomsData.size() >= 2) {
        respondList.room2.maxPlayer = allRoomsData[1].maxPlayer;
        strcpy(respondList.room2.roomName, allRoomsData[1].roomName);
        respondList.room2.roomID = allRoomsData[1].roomID;
        respondList.room2.TheCreatorOfThisRoomIs = allRoomsData[1].TheCreatorOfThisRoomIs;
        respondList.room2.player1 = -1;
        respondList.room2.player2 = -1;
        respondList.room2.player3 = -1;
        respondList.room2.player4 = -1;
        if (allRoomsData[1].playersEnroled.size() >= 1)
            respondList.room2.player1 = allRoomsData[1].playersEnroled[0];
        if (allRoomsData[1].playersEnroled.size() >= 2)
            respondList.room2.player2 = allRoomsData[1].playersEnroled[1];
        if (allRoomsData[1].playersEnroled.size() >= 3)
            respondList.room2.player3 = allRoomsData[1].playersEnroled[2];
        if (allRoomsData[1].playersEnroled.size() >= 4)
            respondList.room2.player4 = allRoomsData[1].playersEnroled[3];
    }
    if (allRoomsData.size() >= 3) {
        respondList.room3.maxPlayer = allRoomsData[2].maxPlayer;
        strcpy(respondList.room3.roomName, allRoomsData[2].roomName);
        respondList.room3.roomID = allRoomsData[2].roomID;
        respondList.room3.TheCreatorOfThisRoomIs = allRoomsData[2].TheCreatorOfThisRoomIs;
        respondList.room3.player1 = -1;
        respondList.room3.player2 = -1;
        respondList.room3.player3 = -1;
        respondList.room3.player4 = -1;
        if (allRoomsData[2].playersEnroled.size() >= 1)
            respondList.room3.player1 = allRoomsData[2].playersEnroled[0];
        if (allRoomsData[2].playersEnroled.size() >= 2)
            respondList.room3.player2 = allRoomsData[2].playersEnroled[1];
        if (allRoomsData[2].playersEnroled.size() >= 3)
            respondList.room3.player3 = allRoomsData[2].playersEnroled[2];
        if (allRoomsData[2].playersEnroled.size() >= 4)
            respondList.room3.player4 = allRoomsData[2].playersEnroled[3];
    }
    if (allRoomsData.size() >= 4) {
        respondList.room4.maxPlayer = allRoomsData[3].maxPlayer;
        strcpy(respondList.room4.roomName, allRoomsData[3].roomName);
        respondList.room4.roomID = allRoomsData[3].roomID;
        respondList.room4.TheCreatorOfThisRoomIs = allRoomsData[3].TheCreatorOfThisRoomIs;
        respondList.room4.player1 = -1;
        respondList.room4.player2 = -1;
        respondList.room4.player3 = -1;
        respondList.room4.player4 = -1;
        if (allRoomsData[3].playersEnroled.size() >= 1)
            respondList.room4.player1 = allRoomsData[3].playersEnroled[0];
        if (allRoomsData[3].playersEnroled.size() >= 2)
            respondList.room4.player2 = allRoomsData[3].playersEnroled[1];
        if (allRoomsData[3].playersEnroled.size() >= 3)
            respondList.room4.player3 = allRoomsData[3].playersEnroled[2];
        if (allRoomsData[3].playersEnroled.size() >= 4)
            respondList.room4.player4 = allRoomsData[3].playersEnroled[3];
    }
    if (allRoomsData.size() >= 5) {
        respondList.room5.maxPlayer = allRoomsData[4].maxPlayer;
        strcpy(respondList.room5.roomName, allRoomsData[4].roomName);
        respondList.room5.roomID = allRoomsData[4].roomID;
        respondList.room5.TheCreatorOfThisRoomIs = allRoomsData[4].TheCreatorOfThisRoomIs;
        respondList.room5.player1 = -1;
        respondList.room5.player2 = -1;
        respondList.room5.player3 = -1;
        respondList.room5.player4 = -1;
        if (allRoomsData[4].playersEnroled.size() >= 1)
            respondList.room5.player1 = allRoomsData[4].playersEnroled[0];
        if (allRoomsData[4].playersEnroled.size() >= 2)
            respondList.room5.player2 = allRoomsData[4].playersEnroled[1];
        if (allRoomsData[4].playersEnroled.size() >= 3)
            respondList.room5.player3 = allRoomsData[4].playersEnroled[2];
        if (allRoomsData[4].playersEnroled.size() >= 4)
            respondList.room5.player4 = allRoomsData[4].playersEnroled[3];
    }

    respondList.nbRoom = allRoomsData.size();
    respondList.type = RoomPacket::RoomGestionType::RespondList;
    respondList.commandId = NetCommand::RoomGestion;
    world.server->Send(&respondList, sizeof(RoomPacket::RoomGestionRespondListPacket), 99, true);
}

std::queue<MGE::ReturnData> &MGE::RoomData::GetReturnData() {
    return roomsDataResponse;
}

void MGE::RoomData::ResponseMessageManager(ReturnData returnData, const World& world) {
    if (returnData.isSucces)
        SendSuccessRoomDataRPC(returnData.message, world);
    else
        SendFailureRoomDataRPC(returnData.message, world);
}

