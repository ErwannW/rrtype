//
// Created by Nail on 15/12/2024.
//

#ifndef DESTROYENTITY_HPP
#define DESTROYENTITY_HPP

#include "NetPacket.hpp"
#include "Network/NetId.hpp"

namespace MGE
{
    NETPACKET(DestroyEntity,
        NetId entityNetId = NetId::None;
    );
} // MGE

#endif //DESTROYENTITY_HPP
