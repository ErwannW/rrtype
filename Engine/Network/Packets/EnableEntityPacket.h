//
// Created by Nail on 1/8/2025.
//

#ifndef ENABLEENTITYPACKET_H
#define ENABLEENTITYPACKET_H

#include "NetPacket.hpp"

namespace MGE
{
    NETPACKET(EnableEntity,
        NetId entityNetId = NetId::None;
        uint8 enabled = true;
    );
} // MGE

#endif //ENABLEENTITYPACKET_H
