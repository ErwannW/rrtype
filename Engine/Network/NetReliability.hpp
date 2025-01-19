//
// Created by np on 12/9/24.
//

#ifndef RTYPE_NETRELIABILITY_HPP
#define RTYPE_NETRELIABILITY_HPP

#include "Types.hpp"

namespace MGE
{
    /**
     * Describes the reliability of a network request.
     */
    enum class NetReliability : uint8
    {
        /**
         * The packet may or may not arrive, not guaranteed to arrive in the order it was sent.
         */
        Unreliable,
        /**
         * The packet is guaranteed to arrive, and in the order it was sent.
         */
        Reliable
    };
} // MGE

#endif //RTYPE_NETRELIABILITY_HPP
