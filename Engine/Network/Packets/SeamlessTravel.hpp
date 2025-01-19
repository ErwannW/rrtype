//
// Created by np on 11/28/24.
//

#ifndef SEEMLESSTRAVEL_HPP
#define SEEMLESSTRAVEL_HPP
#include "NetPacket.hpp"
#include "Hashing/IHashable.hpp"

namespace MGE::NetPackets
{
    struct SeamlessTravel : public NetPacket
    {
        Hash Level;
    };

} // MGE

#endif //SEEMLESSTRAVEL_HPP
