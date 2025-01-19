//
// Created by np on 12/6/24.
//

#ifndef RTYPE_RPC_PACKET_HPP
#define RTYPE_RPC_PACKET_HPP

#include "NetPacket.hpp"
#include "Hashing/Hash.hpp"
#include "Vector/Vector3.hpp"
#include <vector>

namespace MGE {
  struct RPCPacket : public NetPacket
  {
    NetId targetEntity;
    Hash componentHash;
    Hash rpcHash;
  };
}// MGE

#endif //RTYPE_RPC_PACKET_HPP
