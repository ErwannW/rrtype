//
// Created by np on 11/28/24.
//

#include "IHashable.hpp"

namespace MGE
{
    bool IHashable::CompareHash(const IHashable &lhs, const IHashable &rhs)
    {
        return lhs.GetHash() == rhs.GetHash();
    }
} // MGE