//
// Created by np on 11/27/24.
//

#ifndef NETID_H
#define NETID_H

#include <cstdint>
#include <ostream>

namespace MGE
{
    /**
     * An ID used for uniquely identify objects that are replicated over the network.
     */
    enum class NetId : std::uint32_t {
        /**
         * Default, invalid net id.
         */
        None = 0,
    };

    std::string ToString(NetId netId);

    std::ostream &operator<<(std::ostream &os, MGE::NetId netId);
} // MGE

#endif //NETID_H
