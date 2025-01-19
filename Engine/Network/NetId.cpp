//
// Created by np on 11/27/24.
//

#include <string>
#include "NetId.hpp"

namespace MGE {
    std::string ToString(const NetId netId)
    {
        return std::to_string(static_cast<std::underlying_type_t<NetId>>(netId));
    }

    std::ostream &operator<<(std::ostream &os, MGE::NetId netId)
    {
        return os << static_cast<std::underlying_type_t<MGE::NetId>>(netId);
    }
} // MGE