//
// Created by np on 11/28/24.
//

#ifndef RTYPE_SPAWNENTITY_HPP
#define RTYPE_SPAWNENTITY_HPP

#include "NetPacket.hpp"
#include "Hashing/Hash.hpp"

namespace MGE::NetPackets
{
    struct SpawnEntity : public NetPacket
    {
        Hash EntityPreset;
        NetId newEntityId;
    };
} // MGE::NetPackets

#endif //RTYPE_SPAWNENTITY_HPP
