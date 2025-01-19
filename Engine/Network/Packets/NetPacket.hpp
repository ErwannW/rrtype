//
// Created by np on 11/27/24.
//

#ifndef PACKET_HPP
#define PACKET_HPP

#include "Types.hpp"
#include "Network/NetId.hpp"

namespace MGE
{
    #define NETPACKET(s, ...) struct s##Packet : public NetPacket { constexpr s##Packet() : NetPacket(NetCommand::s) { } __VA_ARGS__ }

    enum class NetCommand : uint16 {
        Unknown = 100,
        ReplicateProperty,
        Rpc,
        Travel,
        Game,
        EnableEntity,
        DestroyEntity,
        RoomGestion,
    };

    struct NetPacket
    {
        constexpr NetPacket() = default;
        constexpr explicit NetPacket(const NetCommand command) : commandId(command) {}

        mutable NetCommand commandId = NetCommand::Unknown;
    };
}

#endif //PACKET_HPP
