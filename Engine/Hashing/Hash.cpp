//
// Created by np on 11/28/24.
//

#include "Hash.hpp"
#include "Asserts.hpp"

namespace MGE
{
    std::ostream &operator<<(std::ostream &os, MGE::Hash hash)
    {
        return os << static_cast<std::underlying_type_t<MGE::Hash>>(hash);
    }
} // MGE