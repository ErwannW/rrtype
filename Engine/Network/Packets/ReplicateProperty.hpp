//
// Created by np on 12/5/24.
//

#ifndef RTYPE_REPLICATEPROPERTY_HPP
#define RTYPE_REPLICATEPROPERTY_HPP

#include "NetPacket.hpp"
#include "Hashing/Hash.hpp"
#include "Containers/Buffer.hpp"

namespace MGE
{
    struct ReplicateProperty : public NetPacket
    {
        NetId targetNetId      = NetId::None;
        Hash targetClass    = Hash::None;
        Hash targetProperty = Hash::None;
    };
} // MGE

#endif //RTYPE_REPLICATEPROPERTY_HPP
